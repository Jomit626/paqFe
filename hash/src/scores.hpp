#pragma once

class Scores
{
  unsigned long long confilict_cnt;
  unsigned long long hit_cnt;

  unsigned long long weighted_hit_cnt;
  unsigned long long weighted_confilict_cnt;
public:
  double confilict_rate;
  double hit_rate;
  double weighted_hit_rate;


  void init() {
    confilict_cnt = 0;
    hit_cnt = 0;
    weighted_hit_cnt = 0;
    weighted_confilict_cnt = 0;
  }

  void record_hit(int cnt) {
    hit_cnt++;
    weighted_hit_cnt += 1;
  }

  void record_confilict(int cnt) {
    confilict_cnt++;
    weighted_confilict_cnt += cnt;
  }

  void concrate(unsigned long long n) {
    confilict_rate = double(confilict_cnt) / n;
    hit_rate = double(hit_cnt) / n;

    weighted_hit_rate = double(weighted_hit_cnt) / (weighted_hit_cnt + weighted_confilict_cnt);
  }
};
