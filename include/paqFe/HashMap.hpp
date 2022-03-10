#pragma once

#include <cstddef>
#include <cstring>
#include <cstdlib>

namespace paqFe::internal {

template<typename T, typename Tag, size_t Size>
class HashMap {
protected:
  static constexpr size_t N = Size / sizeof(T);
  static constexpr size_t Mask = N - 1;
  static_assert(isPow2(N));

  T data[N];
  Tag tags[N];

public:
  HashMap() {
    std::memset(data, 0x0, sizeof(data));
    std::memset(tags, 0x0, sizeof(tags));
  }

  bool find(Tag valt, size_t hashval, T** result) {
    hashval &= Mask;
    Tag t = tags[hashval];
    T* entry = &data[hashval];

    *result = entry;

    bool found = true;
    if(valt != t) {
      tags[hashval] = valt;
      std::memset(entry, 0x0, sizeof(T));

      found = false;
    }

    return found;
  }

};

template<typename T, typename Tag, size_t Size, int NWay>
class AssociativeHashMap {
protected:
  static constexpr size_t N = Size / sizeof(T);
  static constexpr size_t NSet = N / NWay;
  static constexpr size_t Mask = NSet - 1;

  static_assert(isPow2(N));
  static_assert(isPow2(NWay));
  static_assert(isPow2(NSet));

  struct Info {
    Tag t;
    uint8_t cnt;
    bool valid;
  };
  
  T data[NSet][NWay];
  Info infos[NSet][NWay];

  void reset_entry(T *entry, Info *info, Tag valt){
    std::memset(entry, 0x0, sizeof(T));
    info->cnt = 0;
    info->t = valt;
    info->valid = true;
  }

  int lru_choose(Info* infos) {
    int max_i, max_cnt = -1;
    for(int i=0;i<NWay;i++) {
      if(!infos[i].valid)
        return i;
      else if(infos[i].cnt > max_cnt) {
        max_i = i;
        max_cnt = infos[i].cnt;
      }
    }

    return max_i;
  }

public:
  AssociativeHashMap() {
    std::memset(data, 0x0, sizeof(data));
    std::memset(infos, 0x0, sizeof(infos));
  }

  bool find(Tag valt, size_t hashval, T** result) {
    hashval &= Mask;
    T *entry_set = &data[hashval][0];
    Info *info_set = &infos[hashval][0];

    bool found = false;
    int hit_i = -1;
    for(int i=0;i<NWay;i++) {
      if(info_set[i].valid && info_set[i].t == valt) {
        hit_i = i;
        found = true;
        break;
      }
    }

    if(found) {
      *result = &entry_set[hit_i];
      if (info_set[hit_i].cnt != 0) {
        info_set[hit_i].cnt = 0;
        for(int i=0;i<NWay;i++) {
          if(i != hit_i) {
            info_set[i].cnt++;
          }
        }
      }
    } else {
      int replace_i = lru_choose(info_set);
      reset_entry(&entry_set[replace_i], &info_set[replace_i], valt);

      *result = &entry_set[replace_i];
    }

    return found;
  }

};

}
