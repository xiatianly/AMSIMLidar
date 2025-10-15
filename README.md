# AMSIM LiDAR Simulation

Real-time, physics-based LiDAR simulation plugin and examples for Unreal Engine.  
Outputs live UDP packets compatible with common viewers (e.g., VeloView), and can log PCAP for offline analysis.

## Features
- Multiple LiDAR models (e.g., VLP-16, Puck-Lite, Puck-HiRes, VLP-32C, VLS-128)
- Weather conditions affecting the point cloud
- Per-triangle ray tracing (supports foliage/branches when meshes use **Complex Collision**)
- intensity & distance
- Live UDP streaming + optional PCAP logging
- Blueprint controls for RPM, FOV, range, beam divergence, rain/fog toggles

## Quick Start
1. **Engine:** Unreal Engine 5.5.x (VS2022 toolchain).
2. **Install plugin:** Copy `/AMSIMLidar` into the folder of Plugins in your UE project. Enable it in *Edit → Plugins*.
3. Download the UDP-Unreal plugin (https://github.com/getnamo/UDP-Unreal/releases/tag/v2.5.0), which is a convenience ActorComponent UDP wrapper for Unreal Engine.
   Copy '/UDP-Unreal' into into the folder of Plugins in your UE project. Both AMSIMLidar and UDP-Unreal should stay in the same foler of Plugins.
4. re-open the UE. UE will compile the Plugins of AMSIMLidar and UDP-Unreal.
5. **Place a sensor:** Drag a LiDAR Blueprint (Velodyne_Lidar) into your level.
6. **Set output:**
   - Local IP / Port (e.g., `192.168.1.201:2368`)
   - Destination IP (e.g., `192.168.1.100`)
   - Optionally enable **PCAP logging** and choose a path.
7. **Viewer (VeloView):**
   - Start VeloView → *Live Sensor* with your IP/port
   - Use the corresponding config file in /VeloView_Config.
   - you will see the point cloud.

<img width="2551" height="1383" alt="Velodyne_VLS_128_4" src="https://github.com/user-attachments/assets/f8b8c2e5-904d-4749-a471-6a02da4d6452" />

<img width="2552" height="1384" alt="Velodyne_VLS_128_5" src="https://github.com/user-attachments/assets/0aafd794-dace-4ed4-9c9a-b97b4074aa82" />

<img width="1908" height="1023" alt="Velodyne_VLS_128_3" src="https://github.com/user-attachments/assets/8047e9f7-209d-4287-9aa9-a95a932d2803" />

<img width="2552" height="1381" alt="Velodyne_VLS_128_8" src="https://github.com/user-attachments/assets/befbfb30-8496-44ea-8022-c33f91e9acd9" />

If any questions or comments, please contact: zhenchun.xia@amsim.ai

This repository is developed on top of MetaLidar (https://github.com/metabotics-ai/MetaLidar), extended for our real-time LiDAR simulation workflow.
