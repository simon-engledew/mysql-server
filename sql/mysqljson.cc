#undef WITH_PERFSCHEMA_STORAGE_ENGINE

#include <iostream>
#include "sql_class.h"
#include "opt_costconstantcache.h"
#include "item_json_func.h"

#ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
#include "../storage/perfschema/pfs_server.h"
#endif /* WITH_PERFSCHEMA_STORAGE_ENGINE */

int main(int _argc, char **_argv)
{
  system_charset_info= &my_charset_utf8mb4_bin;

  const char * name = "mysqljson";

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

  THD *m_thd;
  m_thd= new THD(false);
  THD *stack_thd= m_thd;

  m_thd->set_new_thread_id();
  m_thd->thread_stack= (char*) &stack_thd;
  m_thd->store_globals();

  std::istream_iterator<char> it(std::cin);
  std::istream_iterator<char> end;
  std::string value(it, end);

  Json_dom *dom = Json_dom::parse(value.data(), value.length(), NULL, NULL, false);
  assert(dom);

  String buffer;
  Json_wrapper w(dom);
  w.to_string(&buffer, true, "format");

  std::cout << std::string(buffer.ptr(), buffer.length()) << std::endl;

  m_thd->cleanup_after_query();
  delete m_thd;
}
