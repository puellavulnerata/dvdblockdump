#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <dvdread/dvd_reader.h>

#define dbg_printf(...) if (verbose) printf(__VA_ARGS__)

static int do_dvd_getinfo(const char *path, int verbose);
static int do_dvd_read_and_dump(const char *path, int verbose,
                                uint32_t lb, uint32_t n);
static void dump_block(const unsigned char *block, uint32_t lb);
int main(int argc, char **argv);

static int do_dvd_getinfo(const char *path, int verbose) {
  int rv = 0;
  dvd_reader_t *dvd = NULL;
  uint32_t max_lb;

  dbg_printf("About to DVDOpen(\"%s\")...\n", path);
  dvd = DVDOpen(path);
  dbg_printf("DVDOpen(\"%s\") returned %p\n", path, dvd);
  if (dvd) {
    max_lb = DVDGetMaxLB(dvd);
    dbg_printf("DVDGetMaxLB(%p) returned %u\n", dvd, max_lb);
    printf("The disc at \"%s\" has %u blocks\n", path, max_lb + 1);
    dbg_printf("About to DVDClose(%p)\n", dvd);
    DVDClose(dvd);
    dbg_printf("DVDClose(%p) returned\n", dvd);
  } else {
    printf("Couldn't open \"%s\"\n", path);
    rv = -1;
  }

  return rv;
}

static int do_dvd_read_and_dump(const char *path, int verbose,
                                uint32_t lb, uint32_t n) {
  int rv = 0, encrypted = 1;
  dvd_reader_t *dvd = NULL;
  uint32_t blocks_read, i;
  unsigned char *buffer = NULL;

  if (path && n > 0) {
    dbg_printf("About to DVDOpen(\"%s\")...\n", path);
    dvd = DVDOpen(path);
    dbg_printf("DVDOpen(\"%s\") returned %p\n", path, dvd);
    if (dvd) {
      buffer = malloc(2048 * n);
      if (buffer) {
        dbg_printf("Got buffer of %lu bytes\n", 2048 * n);
        dbg_printf("About to DVDReadRawBlocks(%p, %p, %u, %u, %d)...\n",
                   dvd, buffer, lb, n, encrypted);
        blocks_read = DVDReadRawBlocks(dvd, buffer, lb, n, encrypted);
        printf("Got %u blocks from \"%s\"\n", blocks_read, path);
        for (i = 0; i < blocks_read; ++i) dump_block(buffer + 2048*i, lb + i);
        dbg_printf("DVDReadRawBlocks() returned %u\n", blocks_read);
        free(buffer);
      } else {
        printf("Malloc failed!\n");
        rv = -1;
      }
      dbg_printf("About to DVDClose(%p)\n", dvd);
      DVDClose(dvd);
      dbg_printf("DVDClose(%p) returned\n", dvd);
    } else {
      printf("Couldn't open \"%s\"\n", path);
      rv = -1;
    }
  } else rv = -1;

  return rv;
}

static void dump_block(const unsigned char *block, uint32_t lb) {
  int i, j;
  unsigned char c;

  if (!block) return;

  printf("Block %u:\n", lb);

  for (i = 0; i < 64; ++i) {
    printf("%03x: ", i * 16);
    for (j = 0; j < 16; ++j) {
      c = block[i*16 + j];
      if (j + 1 == 16) printf("%02x\n", c);
      else printf("%02x ", c);
    }
  }
}

int main(int argc, char **argv) {
  const char *path = NULL, *lb_str = NULL, *n_str = NULL;
  unsigned int lb, n;
  int r, do_read = 0;

  if (argc == 3 || argc == 4) {
    if (argc == 4) {
      path = argv[1];
      lb_str = argv[2];
      n_str = argv[3];
    } else {
      path = "/dev/dvd";
      lb_str = argv[1];
      n_str = argv[2];
    }

    r = sscanf(lb_str, "%u", &lb);
    if (r == 1) {
      r = sscanf(n_str, "%u", &n);
      if (r == 1) {
        do_read = 1;
      } else {
        printf("Couldn't parse number of blocks \"%s\"\n", n_str);
      }
    } else {
      printf("Couldn't parse block offset \"%s\"\n", lb_str);
    }
  } else if (argc == 2) path = argv[1];
  else if (argc == 1) path = "/dev/dvd";
  else {
    printf("Wrong number of args\n");
  }

  if (path) {
    if (do_read) return do_dvd_read_and_dump(path, 0, lb, n);
    else return do_dvd_getinfo(path, 0);
  }
  else return -1;
}

