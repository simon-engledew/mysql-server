#ifndef MYSQLJSON_H_
#define MYSQLJSON_H_

#ifdef __cplusplus
extern "C" {
#endif

void init();
char* canonicalize(const char * value, int length);

#ifdef __cplusplus
}
#endif

#endif
