#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
#include <math.h>
#include <stdlib.h>
#include <sys/mman.h>

static int DrawStart(int fd, int x, int y, unsigned short color)
{
		struct fb_var_screeninfo vinfo;
		unsigned short *pfb;
		double alpha = (2 * M_PI) / 10;
		int offset, radius = 12;

		if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0){
				perror("Error reading fixed information");
				return -1;
		}
		pfb = (unsigned short*)mmap(0, vinfo.xres_virtual * vinfo.yres_virtual * 2,
						PROT_READ | PROT_WRITE. MAP_SHARED, fd, 0);

		for(int i = 11; i != 0; i--){
			double r = radius * (i % 2 + 1) / 2;
			double omega = alpha * i;
			int pos = (int)(r * sin(omega) + x
							+ (r * cos(omega) + y) * vinfo.xres_virtual);
			*(pfb + pos) = color;
		}

		mummap(pfb, vinfo.xres * vinfo.yres * 2);

		return 0;
}
