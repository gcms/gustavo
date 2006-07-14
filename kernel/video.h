#ifndef __VIDEO_H__
#define __VIDEO_H__

void
putch(unsigned char c);

void
puts(const char *text);

void
printf(const char *format, ...);

void
settextcolor(unsigned char forecolor, unsigned char backcolor);

void
cls(void);

void
video_init(void);

#endif /* __VIDEO_H__ */
