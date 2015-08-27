// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "content/browser/tracing/background_tracing_config_impl.h"
#include "content/browser/tracing/background_tracing_rule.h"
#include "content/public/test/test_browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

class BackgroundTracingConfigTest : public testing::Test {
 public:
  BackgroundTracingConfigTest()
      : ui_thread_(BrowserThread::UI, &message_loop_) {}

 protected:
  base::MessageLoop message_loop_;
  TestBrowserThread ui_thread_;
};

scoped_ptr<BackgroundTracingConfigImpl> ReadFromJSONString(
    const std::string& json_text) {
  scoped_ptr<base::Value> json_value(base::JSONReader::Read(json_text));

  base::DictionaryValue* dict = NULL;
  if (json_value)
    json_value->GetAsDictionary(&dict);

  scoped_ptr<BackgroundTracingConfigImpl> config(
      static_cast<BackgroundTracingConfigImpl*>(
          BackgroundTracingConfig::FromDict(dict).release()));
  return config;
}

std::string ConfigToString(const BackgroundTracingConfig* config) {
  scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());

  config->IntoDict(dict.get());

  std::string results;
  if (base::JSONWriter::Write(*dict.get(), &results))
    return results;
  return "";
}

std::string RuleToString(const BackgroundTracingRule* rule) {
  scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());

  rule->IntoDict(dict.get());

  std::string results;
  if (base::JSONWriter::Write(*dict.get(), &results))
    return results;
  return "";
}

TEST_F(BackgroundTracingConfigTest, ConfigFromInvalidString) {
  // Missing or invalid mode
  EXPECT_FALSE(ReadFromJSONString("{}"));
  EXPECT_FALSE(ReadFromJSONString("{\"mode\":\"invalid\"}"));
}

TEST_F(BackgroundTracingConfigTest, PreemptiveConfigFromInvalidString) {
  // Missing or invalid category
  EXPECT_FALSE(ReadFromJSONString("{\"mode\":\"preemptive\"}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"invalid\"}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"PREEMPTIVE_TRACING_MODE\", \"category\": "
      "\"invalid\",\"configs\": [{\"rule\": "
      "\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\", \"trigger_name\":\"foo\"}]}"));

  // Missing rules.
  EXPECT_FALSE(
      ReadFromJSONString("{\"mode\":\"PREEMPTIVE_TRACING_MODE\", \"category\": "
                         "\"BENCHMARK\",\"configs\": []}"));

  // Missing or invalid configs
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\"}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": \"\"}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": {}}"));

  // Invalid config entries
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": [{}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": [\"invalid\"]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": [[]]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": [{\"rule\": \"invalid\"}]}"));

  // Missing or invalid keys for a named trigger.
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"preemptive\", \"category\": \"benchmark\","
      "\"configs\": [{\"rule\": \"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\"}]}"));
}

TEST_F(BackgroundTracingConfigTest, ReactiveConfigFromInvalidString) {
  // Missing or invalid configs
  EXPECT_FALSE(ReadFromJSONString("{\"mode\":\"reactive\"}"));
  EXPECT_FALSE(
      ReadFromJSONString("{\"mode\":\"reactive\", \"configs\": \"invalid\"}"));
  EXPECT_FALSE(ReadFromJSONString("{\"mode\":\"reactive\", \"configs\": {}}"));

  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"REACTIVE_TRACING_MODE\", \"configs\": []}"));

  // Invalid config entries
  EXPECT_FALSE(
      ReadFromJSONString("{\"mode\":\"reactive\", \"configs\": [{}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\", \"configs\": [\"invalid\"]}"));

  // Invalid tracing rule type
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": []}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": \"\"}]}"));
  EXPECT_FALSE(
      ReadFromJSONString("{\"mode\":\"reactive\","
                         "\"configs\": [{\"rule\": "
                         "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\"}]}"));

  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", \"category\": "
      "[]}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", \"category\": "
      "\"\"}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", \"category\": "
      "\"benchmark\"}]}"));

  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", \"category\": "
      "\"benchmark\", \"trigger_name\": []}]}"));
  EXPECT_FALSE(ReadFromJSONString(
      "{\"mode\":\"reactive\","
      "\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", \"category\": "
      "\"benchmark\", \"trigger_name\": 0}]}"));
}

TEST_F(BackgroundTracingConfigTest, PreemptiveConfigFromValidString) {
  scoped_ptr<BackgroundTracingConfigImpl> config;

  config = ReadFromJSONString(
      "{\"mode\":\"PREEMPTIVE_TRACING_MODE\", \"category\": "
      "\"BENCHMARK\",\"configs\": [{\"rule\": "
      "\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\", \"trigger_name\":\"foo\"}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::PREEMPTIVE);
  EXPECT_EQ(config->category_preset(), BackgroundTracingConfigImpl::BENCHMARK);
  EXPECT_EQ(config->rules().size(), 1u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"rule\":\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\","
            "\"trigger_name\":\"foo\"}");

  config = ReadFromJSONString(
      "{\"mode\":\"PREEMPTIVE_TRACING_MODE\", \"category\": "
      "\"BENCHMARK\",\"configs\": [{\"rule\": "
      "\"MONITOR_AND_DUMP_WHEN_SPECIFIC_HISTOGRAM_AND_VALUE\", "
      "\"histogram_name\":\"foo\", \"histogram_value\": 1}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::PREEMPTIVE);
  EXPECT_EQ(config->category_preset(), BackgroundTracingConfigImpl::BENCHMARK);
  EXPECT_EQ(config->rules().size(), 1u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"histogram_name\":\"foo\",\"histogram_value\":1,"
            "\"rule\":\"MONITOR_AND_DUMP_WHEN_SPECIFIC_HISTOGRAM_AND_VALUE\"}");

  config = ReadFromJSONString(
      "{\"mode\":\"PREEMPTIVE_TRACING_MODE\", \"category\": "
      "\"BENCHMARK\",\"configs\": [{\"rule\": "
      "\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\", \"trigger_name\":\"foo1\"}, "
      "{\"rule\": \"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\", "
      "\"trigger_name\":\"foo2\"}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::PREEMPTIVE);
  EXPECT_EQ(config->category_preset(), BackgroundTracingConfigImpl::BENCHMARK);
  EXPECT_EQ(config->rules().size(), 2u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"rule\":\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\","
            "\"trigger_name\":\"foo1\"}");
  EXPECT_EQ(RuleToString(config->rules()[1]),
            "{\"rule\":\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\","
            "\"trigger_name\":\"foo2\"}");
}

TEST_F(BackgroundTracingConfigTest, ReactiveConfigFromValidString) {
  scoped_ptr<BackgroundTracingConfigImpl> config;

  config = ReadFromJSONString(
      "{\"mode\":\"REACTIVE_TRACING_MODE\",\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", "
      "\"category\": \"BENCHMARK\", \"trigger_name\": \"foo\"}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::REACTIVE);
  EXPECT_EQ(config->rules().size(), 1u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"category\":\"BENCHMARK\","
            "\"rule\":\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\","
            "\"trigger_name\":\"foo\"}");

  config = ReadFromJSONString(
      "{\"mode\":\"REACTIVE_TRACING_MODE\",\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", "
      "\"category\": \"BENCHMARK_DEEP\", \"trigger_name\": \"foo\"}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::REACTIVE);
  EXPECT_EQ(config->rules().size(), 1u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"category\":\"BENCHMARK_DEEP\","
            "\"rule\":\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\","
            "\"trigger_name\":\"foo\"}");
  config = ReadFromJSONString(
      "{\"mode\":\"REACTIVE_TRACING_MODE\",\"configs\": [{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", "
      "\"category\": \"BENCHMARK_DEEP\", \"trigger_name\": "
      "\"foo1\"},{\"rule\": "
      "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\", "
      "\"category\": \"BENCHMARK_DEEP\", \"trigger_name\": \"foo2\"}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::REACTIVE);
  EXPECT_EQ(config->rules().size(), 2u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"category\":\"BENCHMARK_DEEP\","
            "\"rule\":\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\","
            "\"trigger_name\":\"foo1\"}");
  EXPECT_EQ(RuleToString(config->rules()[1]),
            "{\"category\":\"BENCHMARK_DEEP\","
            "\"rule\":\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\","
            "\"trigger_name\":\"foo2\"}");
  config = ReadFromJSONString(
      "{\"mode\":\"REACTIVE_TRACING_MODE\",\"configs\": [{\"rule\": "
      "\"TRACE_AT_RANDOM_INTERVALS\",\"category\": \"BENCHMARK_DEEP\","
      "\"timeout_min\":10, \"timeout_max\":20}]}");
  EXPECT_TRUE(config);
  EXPECT_EQ(config->tracing_mode(), BackgroundTracingConfig::REACTIVE);
  EXPECT_EQ(config->rules().size(), 1u);
  EXPECT_EQ(RuleToString(config->rules()[0]),
            "{\"category\":\"BENCHMARK_DEEP\",\"rule\":\"TRACE_AT_RANDOM_"
            "INTERVALS\",\"timeout_max\":20,\"timeout_min\":10}");
}

TEST_F(BackgroundTracingConfigTest, ValidPreemptiveConfigToString) {
  scoped_ptr<BackgroundTracingConfigImpl> config(
      new BackgroundTracingConfigImpl(BackgroundTracingConfig::PREEMPTIVE));

  // Default values
  EXPECT_EQ(ConfigToString(config.get()),
            "{\"category\":\"BENCHMARK\",\"configs\":[],\"mode\":\"PREEMPTIVE_"
            "TRACING_MODE\"}");

  // Change category_preset
  config->set_category_preset(BackgroundTracingConfigImpl::BENCHMARK_DEEP);
  EXPECT_EQ(ConfigToString(config.get()),
            "{\"category\":\"BENCHMARK_DEEP\",\"configs\":[],\"mode\":"
            "\"PREEMPTIVE_TRACING_MODE\"}");

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::PREEMPTIVE));
    config->set_category_preset(BackgroundTracingConfigImpl::BENCHMARK_DEEP);

    scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
    dict->SetString("rule", "MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED");
    dict->SetString("trigger_name", "foo");
    config->AddPreemptiveRule(dict.get());

    EXPECT_EQ(ConfigToString(config.get()),
              "{\"category\":\"BENCHMARK_DEEP\",\"configs\":[{\"rule\":"
              "\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\",\"trigger_name\":"
              "\"foo\"}],\"mode\":\"PREEMPTIVE_TRACING_MODE\"}");
  }

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::PREEMPTIVE));
    config->set_category_preset(BackgroundTracingConfigImpl::BENCHMARK_DEEP);

    scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
    dict->SetString("rule", "MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED");
    dict->SetString("trigger_name", "foo1");
    config->AddPreemptiveRule(dict.get());

    dict->SetString("trigger_name", "foo2");
    config->AddPreemptiveRule(dict.get());

    EXPECT_EQ(ConfigToString(config.get()),
              "{\"category\":\"BENCHMARK_DEEP\",\"configs\":[{\"rule\":"
              "\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\",\"trigger_name\":"
              "\"foo1\"},{\"rule\":\"MONITOR_AND_DUMP_WHEN_TRIGGER_NAMED\","
              "\"trigger_name\":\"foo2\"}],\"mode\":\"PREEMPTIVE_TRACING_"
              "MODE\"}");
  }

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::PREEMPTIVE));

    scoped_ptr<base::DictionaryValue> second_dict(new base::DictionaryValue());
    second_dict->SetString(
        "rule", "MONITOR_AND_DUMP_WHEN_SPECIFIC_HISTOGRAM_AND_VALUE");
    second_dict->SetString("histogram_name", "foo");
    second_dict->SetInteger("histogram_value", 1);
    config->AddPreemptiveRule(second_dict.get());

    EXPECT_EQ(ConfigToString(config.get()),
              "{\"category\":\"BENCHMARK\",\"configs\":[{\"histogram_name\":"
              "\"foo\",\"histogram_value\":1,\"rule\":\"MONITOR_AND_DUMP_WHEN_"
              "SPECIFIC_HISTOGRAM_AND_VALUE\"}],\"mode\":\"PREEMPTIVE_TRACING_"
              "MODE\"}");
  }
}

TEST_F(BackgroundTracingConfigTest, InvalidPreemptiveConfigToString) {
  scoped_ptr<BackgroundTracingConfigImpl> config;

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::PREEMPTIVE));

    scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
    dict->SetString("rule", "MONITOR_AND_DUMP_WHEN_BROWSER_STARTUP_COMPLETE");
    config->AddPreemptiveRule(dict.get());

    EXPECT_EQ(ConfigToString(config.get()),
              "{\"category\":\"BENCHMARK\",\"configs\":[],\"mode\":"
              "\"PREEMPTIVE_TRACING_MODE\"}");
  }
}

TEST_F(BackgroundTracingConfigTest, ValidReactiveConfigToString) {
  scoped_ptr<BackgroundTracingConfigImpl> config(
      new BackgroundTracingConfigImpl(BackgroundTracingConfig::REACTIVE));

  // Default values
  EXPECT_EQ(ConfigToString(config.get()),
            "{\"configs\":[],\"mode\":\"REACTIVE_TRACING_MODE\"}");

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::REACTIVE));

    scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
    dict->SetString("rule", "TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL");
    dict->SetString("trigger_name", "foo");
    config->AddReactiveRule(dict.get(),
                            BackgroundTracingConfigImpl::BENCHMARK_DEEP);

    EXPECT_EQ(ConfigToString(config.get()),
              "{\"configs\":[{\"category\":\"BENCHMARK_DEEP\",\"rule\":\"TRACE_"
              "ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\",\"trigger_name\":\"foo\"}]"
              ",\"mode\":\"REACTIVE_TRACING_MODE\"}");
  }

  {
    config.reset(
        new BackgroundTracingConfigImpl(BackgroundTracingConfig::REACTIVE));

    scoped_ptr<base::DictionaryValue> dict(new base::DictionaryValue());
    dict->SetString("rule", "TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL");
    dict->SetString("trigger_name", "foo1");
    config->AddReactiveRule(dict.get(),
                            BackgroundTracingConfigImpl::BENCHMARK_DEEP);

    dict->SetString("trigger_name", "foo2");
    config->AddReactiveRule(dict.get(),
                            BackgroundTracingConfigImpl::BENCHMARK_DEEP);

    EXPECT_EQ(
        ConfigToString(config.get()),
        "{\"configs\":[{\"category\":\"BENCHMARK_DEEP\",\"rule\":\"TRACE_"
        "ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\",\"trigger_name\":\"foo1\"},{"
        "\"category\":\"BENCHMARK_DEEP\",\"rule\":"
        "\"TRACE_ON_NAVIGATION_UNTIL_TRIGGER_OR_FULL\",\"trigger_name\":"
        "\"foo2\"}],\"mode\":\"REACTIVE_TRACING_MODE\"}");
  }
}

}  // namspace content
