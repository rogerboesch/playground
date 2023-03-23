#include "rb_log.h"
#include "rb_base.h"

#ifdef ML_LUMIN
#include <android/log.h>
#endif

void rb_message(const char* msg, int user, int cr) {
	UNUSED_VAR(user);

#ifdef ML_LUMIN
	__android_log_print(ANDROID_LOG_ERROR, (const char*)"rblog", "%s", msg);
#else
	if (cr) printf("rblog> %s\n", msg);
	else printf("rblog> %s", msg);
#endif
}

void rb_message_str1(const char* msg, const char* str1, int user, int cr) {
	UNUSED_VAR(user);

#ifdef ML_LUMIN
	__android_log_print(ANDROID_LOG_ERROR,  (const char*)"rblog", "%s: %s", msg, str1);
#else
	if (cr) printf("rblog> %s: %s\n", msg, str1);
	else printf("rblog> %s: %s", msg, str1);
#endif
}

void rb_message_num1(const char* msg, int num1, int user, int cr) {
	UNUSED_VAR(user);

#ifdef ML_LUMIN
	__android_log_print(ANDROID_LOG_ERROR, (const char*)"rblog", "%s: %d", msg, num1);
#else
	if (cr) printf("rblog> %s: %d\n", msg, num1);
	else printf("rblog> %s: %d", msg, num1);
#endif
}

void rb_message_trace(const char* clazz, const char* function) {
#ifdef ML_LUMIN
    __android_log_print(ANDROID_LOG_ERROR, (const char*)"rblog", "%s::%s()", clazz, function);
#else
    printf("rblog> %s::%s()\n", clazz, function);
#endif

}
