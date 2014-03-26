#
# Copyright (C) 2014 Marcin Chojnacki marcinch7@gmail.com
# Copyright (C) 2014 NovaFusion https://github.com/NovaFusion
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Specify phone tech before including full_phone
$(call inherit-product, vendor/cm/config/gsm.mk)

# Screen
TARGET_SCREEN_HEIGHT := 800
TARGET_SCREEN_WIDTH := 480

# Inherit some common CM stuff
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/samsung/golden/full_golden.mk)

# Device specific
PRODUCT_NAME := cm_golden
PRODUCT_PACKAGES += Torch