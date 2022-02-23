#pragma once

class Scores
{
  unsigned long long confilict_cnt;
  unsigned long long hit_cnt;

  unsigned long long weighted_hit_cnt;
public:
  double confilict_rate;
  double hit_rate;
  double weighted_hit_rate;

  void init() {
    confilict_cnt = 0;
    hit_cnt = 0;
  }

  void record_hit(int cnt) {
    hit_cnt++;
    weighted_hit_cnt += cnt;
  }

  void record_confilict() {
    confilict_cnt++;
  }

  void concrate(unsigned long long n) {
    confilict_rate = double(confilict_cnt) / n;
    hit_rate = double(hit_cnt) / n;

    weighted_hit_rate = double(weighted_hit_cnt) / (weighted_hit_cnt + confilict_cnt * 64);
  }
};
