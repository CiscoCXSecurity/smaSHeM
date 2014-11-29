/*
$Header: /var/lib/cvsd/var/lib/cvsd/smaSHeM/src/smaSHeM.cpp,v 1.5 2014-11-29 19:01:18 timb Exp $

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

(c) Tim Brown, 2013
<mailto:timb@nth-dimension.org.uk>
<http://www.nth-dimension.org.uk/> / <http://www.machine.org.uk/>
*/

#include "smaSHeM.h"

void usage(const char *commandname) {
	if (commandname != (char *) NULL) {
		fprintf(stderr, "usage: %s -v | -i <shmemid> -l <shmemlength> <-@ <patchoffset> -s <patchstring> | -d [-p | -c | -P | -j -x <xstart> -X <endx> -y <starty> -Y <yend>]>\n", commandname);
	} else {
		fprintf(stderr, "usage: (null) -v | -i <shmemid> -l <shmemlength> <-@ <patchoffset> -s <patchstring> | -d [-p | -c | -P | -j -x <xstart> -X <endx> -y <starty> -Y <yend>]>\n");
	}
	exit(EXIT_FAILURE);
}

void error(const char *commandname, const char *errorstring) {
	if (errno) {
		if (errorstring != (char *) NULL) {
			perror(errorstring);
		} else {
			perror("error");
		}
	} else {
		if (errorstring != (char *) NULL) {
			fprintf(stderr, "%s\n", errorstring);
		}
	}
	usage(commandname);
}

int main(int argc, char **argv) {
	int optionflag;
	int versionflag;
	int shmemid;
	int shmemlength;
	int patchoffset;
	char *patchstring;
	int displayflag;
	int perlflag;
	int cflag;
	int prettyflag;
	int jpegflag;
	int xstart;
	int xend;
	int ystart;
	int yend;
	void *shmembuffer;
	int patchcounter;
	int displaycounter;
	char prettybuffer[PRETTYLINELENGTH + 1];
	int xcounter;
	int ycounter;
	int processid;
	int processstatus;
#ifdef WITH_QTGUI
	QImage *qimage;
#endif
	char *filename;
	optionflag = -1;
	versionflag = FALSE;
	shmemid = -1;
	shmemlength = 0;
	patchoffset = 0;
	patchstring = (char *) NULL;
	displayflag = FALSE;
	perlflag = FALSE;
	cflag = FALSE;
	prettyflag = FALSE;
	jpegflag = FALSE;
	xstart = 0;
	xend = 2000;
	ystart = 0;
	yend = 30;
	shmembuffer = (void *) -1;
	patchcounter = 0;
	displaycounter = 0;
	prettybuffer[PRETTYLINELENGTH] = (char) '\x00';
	xcounter = 0;
	ycounter = 0;
	processid = -1;
	processstatus = 0;
#ifdef WITH_QTGUI
	qimage = NULL;
#endif
	filename = (char *) NULL;
	while ((optionflag = getopt(argc, argv, "vi:l:@:s:dpcPjx:X:y:Y:")) != -1) {
		switch (optionflag) {
			case 'v':
				versionflag = TRUE;
				break;
			case 'i':
				shmemid = atoi(optarg);
				break;
			case 'l':
				shmemlength = atoi(optarg);
				break;
			case '@':
				patchoffset = atoi(optarg);
				break;
			case 's':
				patchstring = optarg;
				break;
			case 'd':
				displayflag = TRUE;
				break;
			case 'p':
				perlflag = TRUE;
				break;
			case 'c':
				cflag = TRUE;
				break;
			case 'P':
				prettyflag = TRUE;
				break;
			case 'j':
				jpegflag = TRUE;
				break;
			case 'x':
				xstart = atoi(optarg);
				break;
			case 'X':
				xend = atoi(optarg);
				break;
			case 'y':
				ystart = atoi(optarg);
				break;
			case 'Y':
				yend = atoi(optarg);
				break;
			default:
				error(argv[0], (char *) NULL);
				break;
		}
	}
	if (versionflag == TRUE) {
		printf("%s\n", PACKAGE_STRING);
		printf("(c) Tim Brown, 2013\n");
		printf("<mailto:timb@nth-dimension.org.uk>\n");
		printf("<http://www.nth-dimension.org.uk/> / <http://www.machine.org.uk/>\n");
		exit(EXIT_SUCCESS);
	} else {
		if (shmemid >= 0) {
			if (shmemlength >= 0) {
				if ((patchoffset >=0) && (patchstring != (char *) NULL)) {
					if ((patchoffset + strlen(patchstring)) <= shmemlength) {
						if ((shmembuffer = (void *) shmat(shmemid, (void *) NULL, SHM_RND)) != (void *) -1) {
							if (patchstring != (char *) NULL) {
								for (patchcounter = 0; patchcounter < strlen(patchstring); patchcounter ++) {
									*((char *) shmembuffer + patchoffset + patchcounter) = *(patchstring + patchcounter);
								}
							}
							shmdt(shmembuffer);
						} else {
							error(argv[0], "couldn't map shared memory");
						}
					} else {
						error(argv[0], "couldn't patch beyond mapped memory");
					}
				} else {
					if (displayflag == TRUE) {
						if (shmemlength >= 0) { 
							if ((shmembuffer = (void *) shmat(shmemid, (void *) NULL, SHM_RND | SHM_RDONLY)) != (void *) -1) {
								if (jpegflag != TRUE) {
									for (displaycounter = 0; displaycounter < shmemlength; displaycounter ++) {
										if (perlflag == TRUE) {
											printf("\\x%02x", (unsigned char) *((char *) shmembuffer + displaycounter));
										} else {
											if (cflag == TRUE) {
												printf("0x%02x", (unsigned char) *((char *) shmembuffer + displaycounter));
												if ((displaycounter + 1) < shmemlength) {
													printf(",");
												}
											} else {
												if (prettyflag == TRUE) {
													if ((displaycounter % PRETTYLINELENGTH) == 0) {
														printf("0x%08x\t", shmembuffer + displaycounter);
													}
													if ((displaycounter % PRETTYLINELENGTH) > 0) {
														printf(" ");
													}
													if (isalnum((unsigned char) *((char *) shmembuffer + displaycounter))) {
														prettybuffer[displaycounter % PRETTYLINELENGTH] = (unsigned char) *((char *) shmembuffer + displaycounter);
													} else {
														prettybuffer[displaycounter % PRETTYLINELENGTH] = (unsigned char) '.';
													}
													printf("%02x", (unsigned char) *((char *) (shmembuffer + displaycounter)));
													if ((displaycounter % PRETTYLINELENGTH) == (PRETTYLINELENGTH - 1)) {
														printf("\t%s\n", prettybuffer);
													}
												} else {
													printf("%c", (unsigned char) *((char *) shmembuffer + displaycounter));
												}
											}
										}
									}
								} else {
#ifdef WITH_QTGUI
									if ((xend >= xstart) && (yend >= ystart)) {
										for (xcounter = xstart; xcounter <= xend; xcounter ++) {
											for (ycounter = ystart; ycounter <= yend; ycounter ++) {
												/* we fork here because we have no idea what the correct dimensions of the image actually are */
												processid = fork();
												if (processid == 0) {
													printf("%ix%i", xcounter, ycounter);
													qimage = new QImage((unsigned char *) shmembuffer, xcounter, ycounter, QImage::Format_RGB32);
													filename = (char *) calloc(4 + 1 + 4 + 4 + 1, sizeof(char));
													sprintf(filename, "%i-%i.jpeg", xcounter, ycounter);
													qimage->save(filename, 0, 100);
													free(filename);
													delete qimage;
													printf(" [done]\n");
													exit(EXIT_SUCCESS);
												} else {
													waitpid(processid, &processstatus, 0);
												}
											}
										}
									} else {
										error(argv[0], "dimensions make no sense");
									}
#else
									error(argv[0], "not compiled with --enable-qtgui");
#endif
								}
								shmdt(shmembuffer);
							} else {
								error(argv[0], "couldn't map shared memory");
							}
						} else {
							error(argv[0], "invalid shared memory length");
						}
					} else {
						error(argv[0], "invalid patch string and display flag not set");
					}
				}
			} else {
				error(argv[0], "invalid shared memory length");
			}
		} else {
			error(argv[0], "invalid shared memory ID");
		}
	}
	exit(EXIT_SUCCESS);
}
