#undef WITH_PERFSCHEMA_STORAGE_ENGINE

#include "mysqljson.h"
#include <iostream>
#include "sql_class.h"
#include "opt_costconstantcache.h"
#include "item_json_func.h"

#ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
#include "../storage/perfschema/pfs_server.h"
#endif /* WITH_PERFSCHEMA_STORAGE_ENGINE */

#ifdef __cplusplus
extern "C" {
#endif

void init()
{
  system_charset_info= &my_charset_utf8mb4_bin;

  const char * name = "libmysqljson";

  char *argv[] = { const_cast<char*>(name), 0 };
  set_remaining_args(1, argv);

  #ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
  pre_initialize_performance_schema();
  #endif /*WITH_PERFSCHEMA_STORAGE_ENGINE */

  MY_INIT(name);

  sys_var_init();
  init_common_variables();
  randominit(&sql_rand, 0, 0);
  transaction_cache_init();
  init_optimizer_cost_module(false);
}

char* canonicalize(const char * value, int length)
{
  THD thd(false);
  THD *stack_thd= &thd;

  thd.set_new_thread_id();
  thd.thread_stack= (char*) &stack_thd;
  thd.store_globals();

  String buffer;

  Json_dom *dom = Json_dom::parse(value, length, NULL, NULL, false);
  if (dom == NULL) {
    return buffer.c_ptr();
  }

  Json_wrapper w(dom);
  w.to_string(&buffer, true, "format");

  thd.cleanup_after_query();

  return buffer.c_ptr();
}

#ifdef __cplusplus
}
#endif
