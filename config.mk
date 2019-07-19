# Name: Makefile_STM
# Author: Daniel Nery Silva de Oliveira
# ThundeRatz Robotics Team
# 06/2019

# Cube file name without .ioc extension
PROJECT_NAME = board

DEVICE_FAMILY  := STM32F4xx
DEVICE_TYPE    := STM32F401xx
DEVICE_DEF     := STM32F401xE
DEVICE         := STM32F401RE

# Linker script file without .ld extension
# Find it on cube folder after code generation
DEVICE_LD_FILE := STM32F401RETx_FLASH

# Lib dir
LIB_DIR  := lib

# Cube Directory
CUBE_DIR := cube

# Default values, can be set on the command line or here
DEBUG   ?= 0
VERBOSE ?= 0
