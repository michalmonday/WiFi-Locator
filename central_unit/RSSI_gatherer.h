#ifndef RSSI_GATHERER_H
#define RSSI_GATHERER_H

#include <Arduino.h>

#define RSSI_MODE_REAL_TIME 1
#define RSSI_MODE_MEAN 2
#define RSSI_MODE_GPS 3


class RSSI_gatherer
{
public:
  void Init(char *ssid, char *password);
  void Update();
  void Draw();
  void Reset();
  void BeginGathering()  { gather_mean_end = millis() + gathering_time; }
  void SetMeanGatheringTime(int seconds)  { gathering_time = seconds * 1000; }
  int GetMeanGatheringTime()  { return gathering_time / 1000; }
  void SetMode(int mode_id)  { rssi_mode = mode_id; }

  int GetNodeRSSI(int node_id)  { return rssi[node_id]; }
  int GetNodeLastUpdateTime(int node_id)  { return last_rec[node_id]; }
  int GetNodeMeanRSSI(int node_id)  { return last_mean_rssi[node_id]; }
  int GetNodeMeanReadingsCount(int node_id)  { return rssi_total_count[node_id]; }

  bool IsMeanBeingGathered()  { return (bool)gather_mean_end; }
  
private:  
  int rssi_mode = RSSI_MODE_REAL_TIME;
  unsigned long gather_mean_end = 0;
  unsigned long gathering_time = 0;
  
  int rssi[4];
  int last_mean_rssi[4];
  unsigned long last_rec[4];  // time (used to display circle if node didn't provide rssi reading for some time)
  
  // total vars used for checking mean rssi over some period of time
  int rssi_total[4];
  int rssi_total_count[4];

  String gps_line = "";


  void DrawRSSI(bool use_mean);
  void HandleButtons();
  void CalculateMean();
};








#endif
