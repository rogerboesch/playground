//
//  rb_log.h
//  Game engine base code
//
//  Base types
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_LOG_H
#define RB_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

	void rb_message(const char* msg, int user, int cr);
	void rb_message_str1(const char* msg, const char* str1, int user, int cr);
	void rb_message_num1(const char* msg, int num1, int user, int cr);
	void rb_message_fnum1(const char* msg, float num1, int user, int cr);
	void rb_message_trace(const char* clazz, const char* function);

#ifdef __cplusplus
}
#endif

#define RBTRACELOG(clazz, function) rb_message_trace(clazz, function);

// Messages added to log only (CR is added)
#define RBLOG(msg) rb_message(msg, 0, 1);
#define RBLOG_STR1(msg, str1) rb_message_str1(msg, str1, 0, 1);
#define RBLOG_NUM1(msg, num1) rb_message_num1(msg, num1, 0, 1);
#define RBLOG_FLOAT1(msg, num1) rb_message_fnum1(msg, num1, 0, 1);

// Messages are showed to user and added to log (CR is added)
#define RBMSG(msg) rb_message(msg, true, true);
#define RBMSG_STR1(msg, str1) rb_message_str1(msg, str1, 1, 1);
#define RBMSG_NUM1(msg, num1) rb_message_num1(msg, num1, 1, 1);

// Print functionality (To print CR use rbprint_newline)
#define RBPRINT(str) rb_message(str, 1, 0);
#define RBPRINT_NEWLINE(str) rb_message(str, 1, 1);
#define RBPRINT_NUM(num) rb_message_num1("", num, 1, 0);

#endif
