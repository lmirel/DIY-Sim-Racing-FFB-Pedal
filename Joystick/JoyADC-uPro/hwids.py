Import("env")

print(env.Dump())

board_config = env.BoardConfig()
# should be array of VID:PID pairs
board_config.update("build.hwids", [
  ["0xAD4F", "0xD4F0"]
])
board_config.update("build.usb_product", "FFB-Pedal-Joy")
board_config.update("build.usb_manufacturer", "EH:D4M - Design 4 fun")
