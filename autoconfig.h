#ifndef watchsettings_h
#define watchsettings_h

#include <pebble.h>

#define {{ uppercase (cvarname oauth.id) }}/*cvarname & upper */_PKEY {{ appKeysAccessor oauth.id }}/*json mapping of appKeys given a name. -> return appKeys[lowercase(input)] */
char* get{{ capitalizeFirst (cvarname oauth.id) }}Token/* cvarname & capitalize */();
{{#each settings.settingsList}}
  {{#is type "toggle"}}
  /*this defines the mapping between dev friendly name and memory friendly ID*/
  #define {{ uppercase (cvarname data.id) }}/*cvarname & upper */_PKEY {{ appKeysAccessor data.id }}/*json mapping of appKeys given a name */
  bool get{{ capitalizeFirst (cvarname data.id) }}/* cvarname & capitalize */();
  {{/is}}
  {{#is type "input"}}
  /*this defines the mapping between dev friendly name and memory friendly ID*/
  #define {{ uppercase (cvarname data.id) }}/*cvarname & upper */_PKEY {{ appKeysAccessor data.id }}/*json mapping of appKeys given a name */
  char* get{{ capitalizeFirst (cvarname data.id) }}/* cvarname & capitalize */();
  {{/is}}
  {{#is type "radio"}}
  /*this defines the mapping between dev friendly name and memory friendly ID*/
  #define {{ uppercase (cvarname data.id) }}/*cvarname & upper */_PKEY {{ appKeysAccessor data.id }}/*json mapping of appKeys given a name */
  char* get{{ capitalizeFirst (cvarname data.id) }}/* cvarname & capitalize */();
  {{/is}}
{{/each}}

void autoconfig_in_received_handler(DictionaryIterator *iter, void *context);

void autoconfig_init();

void autoconfig_deinit();

#endif