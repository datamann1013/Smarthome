

To get Kali Linux for raspberry pi
https://www.kali.org/get-kali/#kali-arm
Install and use  raspberry pi imager
https://www.raspberrypi.com/software/
Put it onto an SD card
If screen isnt booting

```bash
  # Change this:
  dtoverlay=vc4-kms-v3d
  # To this:
  dtoverlay=vc4-fkms-v3d  # Or comment it out entirely with `#`
```
  ```bash
  hdmi_force_hotplug=1  # Force HDMI output even if not detected
  hdmi_group=2          # Use standard monitor timings
  hdmi_mode=82          # 1920x1080 @ 60Hz (adjust mode as needed)
  # hdmi_safe=1         # Uncomment for a "safe" mode if other fixes fail
```

Update the system
```bash
  sudo apt update && sudo apt upgrade
```

Install CPU power management
```bash
  sudo apt install linux-cpupower
```

For a fixed frequency
```bash
  sudo cpupower frequency-set --min 600MHz --max 600MHz --governor powersave
```
For a ondemand set frequency

```bash
  sudo cpupower frequency-set --min 600MHz --max 1200MHz --governor ondemand
```

Configure throttling
Go into (the file at /boot/config.txt has been moved)
```bash
  sudo nano /boot/firmware/config.txt
```

and add
```bash
  arm_freq_min=300  # Min CPU frequency (MHz)
  gpu_freq_min=200  # Min GPU frequency
  avoid_warnings=2  # Allow low-voltage operation without warnings
```

It could be viable to disable the HDMI if not needed. Potentially find a solution to turn off when not in use.

Disable onboard leds permanently
```bash
  # Disable onboard LEDs (permanent)
  echo "dtparam=act_led_trigger=none" | sudo tee -a /boot/firmware/config.txt
  echo "dtparam=act_led_activelow=on" | sudo tee -a /boot/firmware/config.txt

USB power down when idle
```bash
  sudo nano /etc/udev/rules.d/50-usb_power_save.rules
```

Write in it
```bash
  ACTION=="add", SUBSYSTEM=="usb", TEST=="power/control", ATTR{power/control}="auto"
```

Restart it to get teh effects
```bash
  sudo udevadm control --reload
```

Small power tweaks in kernel
```bash  
  sudo nano /etc/sysctl.conf

  # Reduce power usage
  vm.dirty_writeback_centisecs=1500
  vm.laptop_mode=5
```

Check the results with powertop
```bash
  # Install monitoring tools
  sudo apt install powertop

  # Run power audit (requires 1-2 min)
  sudo powertop --auto-tune  # Applies temporary optimizations
  sudo powertop --html       # Generates report
```

Check the clock speed
```bash
  # Real-time CPU frequency
  watch -n1 "cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
```

Make persisten power settings

```bash
  sudo mkdir -p /var/cache/powertop
  sudo powertop --auto-tune 2>&1 | grep -v "failed" | sudo tee /var/cache/powertop/saved_parameters.powertop
  sudo chmod 600 /var/cache/powertop/saved_parameters.powertop
```

Make a systemd service for reboot setting 
```bash
  sudo nano /etc/systemd/system/powertoptune.service
```

Put this into the file
```bash
  [Unit]
  Description=Powertop Auto Tune
  After=multi-user.target

  [Service]
  Type=oneshot
  ExecStart=/usr/sbin/powertop --auto-tune
  RemainAfterExit=yes

  [Install]
  WantedBy=multi-user.target
```

Then run these to make it run at boot and now

```bash
  sudo systemctl daemon-reload
  sudo systemctl enable powertoptune.service
  sudo systemctl start powertoptune.service
```

Disable the hardware watchdog timer
```bash
  sudo nano /etc/default/watchdog
```

change or add
```bash
  run_watchdog=0
```

Disable the service
```bash
  sudo systemctl disable watchdog
  sudo systemctl stop watchdog
```
If you do not plan to have anything connected through USB you can save extra by adding to /boot/cmdline.txt
```bash
dwc_otg.lpm_enable=1
```

Undervolt to reduce heat
```bash
  echo "over_voltage=-2" | sudo tee -a /boot/firmware/config.txt

Process scheduling
```bash
  sudo apt install schedtool
  echo '*       hard    rtprio  0' | sudo tee -a /etc/security/limits.conf
```

If you want a way to test your changes:
```bash
  # Verify all services:
  systemctl status powertoptune.service

  # Check power states:
  sudo powertop --html=powerreport.html

  # Monitor realtime consumption:
  sudo apt install sysbench
  sudo sysbench cpu --threads=1 --time=30 run
  watch -n1 "vcgencmd measure_clock arm; vcgencmd measure_volts"
```