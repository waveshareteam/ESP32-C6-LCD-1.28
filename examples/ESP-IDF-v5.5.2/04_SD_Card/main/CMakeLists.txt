# 你的原有内容
file(GLOB_RECURSE MAIN_SRCS ./*.cpp ./*.c)
set(EMBEDDED_FILES "")

idf_component_register(
    SRCS ${MAIN_SRCS}
    INCLUDE_DIRS "."
    REQUIRES esp32_c6_touch_lcd_1_28
)
