Import("env")

print(env.Dump())

board_config = env.BoardConfig()
# should be array of VID:PID pairs
board_config.update("build.hwids", [
  ["0xCAFE", "0xF011"]
])
board_config.update("build.usb_product", "FFB-Pedal")
board_config.update("build.usb_manufacturer", "DIY")
