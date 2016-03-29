#ifndef CHARSET_H
#define CHARSET_H

#if CYGWIN_VERSION_DLL_MAJOR >= 1007
  #define HAS_LOCALES 1
#else
  #define HAS_LOCALES 0
#endif

static inline wchar
high_surrogate(xchar xc)
{ return 0xD800 | (((xc - 0x10000) >> 10) & 0x3FF); }

static inline wchar
low_surrogate(xchar xc)
{ return 0xDC00 | (xc & 0x3FF); }

static inline bool
is_high_surrogate(wchar wc)
{ return (wc & 0xFC00) == 0xD800; }

static inline bool
is_low_surrogate(wchar wc)
{ return (wc & 0xFC00) == 0xDC00; }

static inline xchar
combine_surrogates(wchar hwc, wchar lwc)
{ return 0x10000 + ((hwc & 0x3FF) << 10) + (lwc & 0x3FF); }

extern void cs_init(void);
extern void cs_reconfig(void);

extern string cs_lang(void);

extern string cs_get_locale(void);
extern void cs_set_locale(string);

typedef enum { CSM_DEFAULT, CSM_OEM, CSM_UTF8 } cs_mode;
extern void cs_set_mode(cs_mode);

extern int cs_wcntombn(char *s, const wchar *ws, size_t len, size_t wlen);
extern int cs_wcstombs(char *s, const wchar *ws, size_t len);
extern int cs_mbstowcs(wchar *ws, const char *s, size_t wlen);
extern int cs_mb1towc(wchar *pwc, char c);
extern wchar cs_btowc_glyph(char);

extern char * cs__wcstombs(const wchar * ws);
extern char * cs__wcstombs_dropill(const wchar * ws);
extern char * cs__wcstoutf(const wchar * ws);
extern wchar * cs__mbstowcs(const char * s);
extern wchar * cs__utftowcs(const char * s);
extern wchar * cs__utforansitowcs(const char * s);

extern string locale_menu[];
extern string charset_menu[];

extern int cs_cur_max;

extern bool font_ambig_wide;

#if HAS_LOCALES
extern bool cs_ambig_wide;
#else
#define cs_ambig_wide font_ambig_wide
extern int xcwidth(xchar c);
#endif

#endif


#ifdef __CYGWIN__
#include <sys/cygwin.h>
# if CYGWIN_VERSION_API_MINOR >= 181

static inline char *
path_win_w_to_posix(const wchar * wp)
{ return cygwin_create_path(CCP_WIN_W_TO_POSIX, wp); }

static inline wchar *
path_posix_to_win_w(const char * p)
{ return cygwin_create_path(CCP_POSIX_TO_WIN_W, p); }

static inline char *
path_posix_to_win_a(const char * p)
{ return cygwin_create_path(CCP_POSIX_TO_WIN_A, p); }

# else
#include <winbase.h>
#include <winnls.h>

static inline char *
path_win_w_to_posix(const wchar * wp)
{
  char * mp = cs__wcstombs(wp);
  char * p = newn(char, MAX_PATH);
  cygwin_conv_to_full_posix_path(mp, p);
  free(mp);
  p = renewn(p, strlen(p) + 1);
  return p;
}

static inline wchar *
path_posix_to_win_w(const char * p)
{
  char ap[MAX_PATH];
  cygwin_conv_to_win32_path(p, ap);
  wchar * wp = newn(wchar, MAX_PATH);
  MultiByteToWideChar(0, 0, ap, -1, wp, MAX_PATH);
  wp = renewn(wp, wcslen(wp) + 1);
  return wp;
}

static inline char *
path_posix_to_win_a(const char * p)
{
  char * ap = newn(char, MAX_PATH);
  cygwin_conv_to_win32_path(p, ap);
  ap = renewn(ap, strlen(ap) + 1);
  return ap;
}

# endif
#else

#warning port to midipix...

#endif


# ifdef TEST_WCS
#define wcsdup _wcsdup
#define wcschr _wcschr
#define wcsrchr _wcsrchr
#define wcsncmp _wcsncmp
# endif

#if CYGWIN_VERSION_API_MINOR < 74 || defined(TEST_WCS)
// needed for MinGW MSYS

#define wcscpy(tgt, src) memcpy(tgt, src, (wcslen(src) + 1) * sizeof(wchar))

static wchar *
wcschr(const wchar * s, wchar c)
{
  while (* s) {
    if (* s == c)
      return (wchar *)s;
    s ++;
  }
  return 0;
}

static wchar *
wcsrchr(const wchar * s, wchar c)
{
  wchar * res = 0;
  while (* s) {
    if (* s == c)
      res = (wchar *)s;
    s ++;
  }
  return res;
}

static int
wcsncmp(const wchar * s1, const wchar * s2, int len)
{
  for (int i = 0; i < len; i++)
    if (s1[i] < s2[i])
      return -1;
    else if (s1[i] > s2[i])
      return 1;
    else if (s1[i] == 0)
      return 0;
  return 0;
}

#endif

#if CYGWIN_VERSION_API_MINOR < 207 || defined(TEST_WCS)

static wchar *
wcsdup(const wchar * s)
{
  wchar * dup = newn(wchar, wcslen(s) + 1);
  wcscpy(dup, s);
  return dup;
}

static void
_unused(void)
{
  (void)_unused;
  (void)wcsdup;
  (void)wcschr;
  (void)wcsrchr;
  (void)wcsncmp;
}

#endif

