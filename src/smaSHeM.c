/*
$Header: /var/lib/cvsd/var/lib/cvsd/smaSHeM/src/Attic/smaSHeM.c,v 1.2 2013-01-02 09:26:06 timb Exp $

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

(c) Tim Brown, 2012
<mailto:timb@nth-dimension.org.uk>
<http://www.nth-dimension.org.uk/> / <http://www.machine.org.uk/>
*/

#include "smaSHeM.h"

void usage(char *commandname) {
	/* TODO enable -a */
	if (commandname != (char *) NULL) {
		fprintf(stderr, "usage: %s -i <shmemid> -l <shmemlength> <-@ <patchoffset> -s <patchstring> | -d [-p | -c | -P]>\n", commandname);
	} else {
		fprintf(stderr, "usage: (null) -i <shmemid> -l <shmemlength> <-@ <patchoffset> -s <patchstring> | -d [-p | -c | -P]>\n");
	}
	exit(EXIT_FAILURE);
}

void error(char *commandname, char *errorstring) {
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
	int shmemid;
	int shmemlength;
	int patchoffset;
	char *patchstring;
	int displayflag;
	int perlflag;
	int cflag;
	int prettyflag;
	void *shmembuffer;
	int patchcounter;
	int displaycounter;
	int blockflag;
	int blockstart;
	int blockend;
	char prettybuffer[PRETTYLINELENGTH + 1];
	optionflag = -1;
	shmemid = -1;
	shmemlength = 0;
	patchoffset = 0;
	patchstring = (char *) NULL;
	displayflag = FALSE;
	perlflag = FALSE;
	cflag = FALSE;
	prettyflag = FALSE;
	shmembuffer = (void *) -1;
	patchcounter = 0;
	displaycounter = 0;
	blockflag = FALSE;
	blockstart = 0;
	blockend = 0;
	prettybuffer[PRETTYLINELENGTH] = (char) '\x00';
	while ((optionflag = getopt(argc, argv, "i:l:@:s:dpcP")) != -1) {
		switch (optionflag) {
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
			default:
				error(argv[0], (char *) NULL);
				break;
		}
	}
	if (shmemid >= 0) {
		if (shmemlength >= 0) {
			if ((patchoffset >=0) && (patchstring != (char *) NULL)) {
				if ((patchoffset + strlen(patchstring)) <= shmemlength) {
					if ((shmembuffer = (void *) shmat(shmemid, (void *) NULL, SHM_RND)) != (void *) -1) {
						if (patchstring != (char *) NULL) {
							for (patchcounter = 0; patchcounter < strlen(patchstring); patchcounter ++) {
								*((char *) (shmembuffer + patchoffset + patchcounter)) = *(patchstring + patchcounter);
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
							for (displaycounter = 0; displaycounter < shmemlength; displaycounter ++) {
								if (perlflag == TRUE) {
									printf("\\x%02x", (unsigned char) *((char *) (shmembuffer + displaycounter)));
								} else {
									if (cflag == TRUE) {
										printf("0x%02x", (unsigned char) *((char *) (shmembuffer + displaycounter)));
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
											if (isalnum((unsigned char) *((char *) (shmembuffer + displaycounter)))) {
												prettybuffer[displaycounter % PRETTYLINELENGTH] = (unsigned char) *((char *) (shmembuffer + displaycounter));
											} else {
												prettybuffer[displaycounter % PRETTYLINELENGTH] = (unsigned char) '.';
											}
											printf("%02x", (unsigned char) *((char *) (shmembuffer + displaycounter)));
											if ((displaycounter % PRETTYLINELENGTH) == (PRETTYLINELENGTH - 1)) {
												printf("\t%s\n", prettybuffer);
											}
										} else {
											printf("%c", (unsigned char) *((char *) (shmembuffer + displaycounter)));
										}
									}
								}
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
	exit(EXIT_SUCCESS);
}
