package com.moca.rpc.protocol;

import java.util.*;

public final class KeyValuePair
{
  private String key;
  private String value;

  public KeyValuePair(String k, String v)
  {
    this.key = k;
    this.value = v;
  }

  public String getKey()
  {
    return key;
  }

  public String key()
  {
    return key;
  }

  public String getValue()
  {
    return value;
  }

  public String value()
  {
    return value;
  }

  public static Map<String, String> toMap(KeyValuePair[] pairs, boolean ignoreDuplicate)
  {
    HashMap<String, String> result = new HashMap(pairs.length);
    if (ignoreDuplicate) {
      for (KeyValuePair pair : pairs) {
        result.put(pair.key(), pair.value());
      }
    } else {
      for (KeyValuePair pair : pairs) {
        String key = pair.key();
        if (result.containsKey(key)) {
          throw new RuntimeException("Key '" + key + "' has multiple entries");
        }
        result.put(key, pair.value());
      }
    }
    return result;
  }

  public static Map<String, String> toMap(KeyValuePair[] pairs)
  {
    return toMap(pairs, false);
  }

  public static KeyValuePair[] create(Map<String, String> items)
  {
    KeyValuePair[] pairs = new KeyValuePair[items.size()];
    int idx = 0;
    for (Map.Entry<String, String> entry : items.entrySet()) {
      pairs[idx++] = new KeyValuePair(entry.getKey(), entry.getValue());
    }
    return pairs;
  }

  public static KeyValuePair[] create(String ... items)
  {
    if (items.length % 2 != 0) {
      throw new IllegalArgumentException("Arguments must be paired");
    }
    int size = size = items.length / 2;
    KeyValuePair[] pairs = new KeyValuePair[size];
    for (int idx = 0; idx < size; ++idx) {
      int offset = 2 * idx;
      pairs[idx] = new KeyValuePair(items[offset], items[offset + 1]);
    }
    return pairs;
  }

  public String toString()
  {
    return "(" + key + " => " + value + ")";
  }
}
