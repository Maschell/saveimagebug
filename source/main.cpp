#include <coreinit/systeminfo.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>

#include <whb/log.h>
#include <whb/log_console.h>
#include <whb/proc.h>

#include <sys/stat.h>

#include <gd.h>

int
main(int argc, char **argv)
{
   WHBProcInit();
   WHBLogConsoleInit();

   gdImagePtr im = gdImageCreateTrueColor(400, 400);
   if (!im) {
      WHBLogPrintf("Error: Unable to create image.");
      return 1;
   }

   int white = gdImageColorAllocate(im, 255, 255, 255);
   int blue  = gdImageColorAllocate(im, 0, 0, 255);
   int red   = gdImageColorAllocate(im, 255, 0, 0);

   gdImageFill(im, 0, 0, white);
   gdImageArc(im, 200, 200, 300, 300, 0, 360, blue);
   gdImageLine(im, 0, 0, 399, 399, red);

   FILE* jpegOut = fopen("output.jpg", "wb");
   if (jpegOut) {
      gdImageJpeg(im, jpegOut, 90);
      fclose(jpegOut);

      WHBLogPrintf("Successfully saved output.jpg");

      struct stat statbuf {};
      if (stat("output.jpg", &statbuf) >= 0) {
         WHBLogPrintf("Filesize: %lld",statbuf.st_size);
      } else {
         WHBLogPrintf("Failed to get filesize");
      }
   }

   gdImageDestroy(im);

   struct stat statbuf {};
   stat("output.jpg", &statbuf);

   WHBLogConsoleDraw();

   while (WHBProcIsRunning()) {
      OSSleepTicks(OSMillisecondsToTicks(100));
   }

   WHBLogConsoleFree();
   WHBProcShutdown();

   return 0;
}
