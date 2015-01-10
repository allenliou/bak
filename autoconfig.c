#include <pebble.h>
#include "autoconfig.h"

{{#each settings.settingsList}}
{{#is type "toggle"}}
bool _{{ cvarname data.id }};
bool get{{ capitalizeFirst (cvarname data.id) }}(){return _{{ cvarname data.id }};}
void set{{ capitalizeFirst (cvarname data.id) }}(bool value){_{{ cvarname data.id }} = value;}
{{addToPersistant 6 "bool"}}/*4 + 2*/

{{/is}}
{{#is type "input"}}
char _{{ cvarname data.id }}[{{item['max-length']}}];
char* get{{ capitalizeFirst cvarname data.id }}(){return _{{ cvarname data.id }};}
void set{{ capitalizeFirst cvarname data.id }}(char* value){strncpy(_{{ cvarname data.id }}, value, {{ item['max-length'] }});}
/*{% set persitantDataSize = persitantDataSize + 4 + 1 * (item['max-length']|int) %}*/
{{/is}}
{{#is type "radio"}}
char _{{ cvarname data.id }}[{{item['max-length']}}];
char* get{{ capitalizeFirst cvarname data.id }}(){return _{{ cvarname data.id }};}
void set{{ capitalizeFirst cvarname data.id }}(char* value){strncpy(_{{ cvarname data.id }}, value, {{ item['max-length'] }});}
/*{% set persitantDataSize = persitantDataSize + 4 + 1 * (item['max-length']|int) %}*/
{{/is}}
{{/each}}

void autoconfig_in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *tuple = NULL;
  {{#each settings.settingsList}}
  tuple = dict_find(iter, {{ uppercase (cvarname data.id) }}_PKEY);
  {{#is type "input"}}
  tuple ? set{{ capitalizeFirst (cvarname data.id) }}(tuple->value->cstring) : false;
  {{/is}}
  {{#is type "radio"}}
  tuple ? set{{ capitalizeFirst (cvarname data.id) }}(tuple->value->cstring) : false;
  {{/is}}
  {{/each}}
}

void autoconfig_init(){
  app_message_register_inbox_received(autoconfig_in_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  {{#each settings.settingsList}}
  if (persist_exists({{ uppercase (cvarname data.id) }}_PKEY)) {
    {{#is type "input"}}
    persist_read_string({{ uppercase (cvarname data.id) }}_PKEY, _{{ cvarname data.id }}, {{item['max-length'] + 1}});
    set{{ capitalizeFirst (cvarname data.id) }}(_{{ cvarname data.id }});

    {{/is}}
    {{#is type "radio"}}
    persist_read_string({{ uppercase (cvarname data.id) }}_PKEY, _{{ cvarname data.id }}, {{item['max-length'] + 1}});
    set{{ capitalizeFirst (cvarname data.id) }}(_{{ cvarname data.id }});

    {{/is}}
    {{#is type "toggle"}}
    set{{ capitalizeFirst (cvarname data.id) }}(persist_read_bool({{ uppercase (cvarname data.id) }}_PKEY));

    {{/is}}
  }
  {{/each}}
}

void autoconfig_deinit(){
  {{#each settings.settingsList}}
  {{#is type "radio"}}
  persist_write_string({{ uppercase (cvarname data.id) }}_PKEY, _{{ cvarname data.id }});

  {{/is}}
  {{#is type "input"}}
  persist_write_string({{ uppercase (cvarname data.id) }}_PKEY, _{{ cvarname data.id }});

  {{/is}}
  {{#is type "toggle"}}
  persist_write_bool({{ uppercase (cvarname data.id) }}_PKEY, _{{ cvarname data.id }});

  {{/is}}
  {{/each}}
}