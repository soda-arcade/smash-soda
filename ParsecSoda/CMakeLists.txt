set(PROJECT_NAME SmashSoda)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "AGamepad.h"
    "AudioIn.h"
    "AudioMix.h"
    "AudioOut.h"
    "AudioTools.h"
    "BanList.h"
    "Base64.h"
    "Bitwise.h"
    "ButtonLock.h"
    "ChatBot.h"
    "ChatLog.h"
    "Clock.h"
    "Commands/ACommand.h"
    "Commands/ACommandIntegerArg.h"
    "Commands/ACommandPrefix.h"
    "Commands/ACommandSearchUser.h"
    "Commands/ACommandSearchUserHistory.h"
    "Commands/ACommandSearchUserIntArg.h"
    "Commands/ACommandStringArg.h"
    "Commands/Command1v1.h"
    "Commands/Command2v2.h"
    "Commands/CommandAFK.h"
    "Commands/CommandBan.h"
    "Commands/CommandBB.h"
    "Commands/CommandBonk.h"
    "Commands/CommandBotMessage.h"
    "Commands/CommandDC.h"
    "Commands/CommandDefaultMessage.h"
    "Commands/CommandDiscord.h"
    "Commands/CommandFF.h"
    "Commands/CommandGameId.h"
    "Commands/CommandGuests.h"
    "Commands/CommandHelp.h"
    "Commands/CommandIpFilter.h"
    "Commands/CommandJoin.h"
    "Commands/CommandKick.h"
    "Commands/CommandKOTH.h"
    "Commands/CommandLimit.h"
    "Commands/CommandMic.h"
    "Commands/CommandMirror.h"
    "Commands/CommandMod.h"
    "Commands/CommandName.h"
    "Commands/CommandOne.h"
    "Commands/CommandPads.h"
    "Commands/CommandPing.h"
    "Commands/CommandPingAll.h"
    "Commands/CommandPrivate.h"
    "Commands/CommandPublic.h"
    "Commands/CommandQuit.h"
    "Commands/CommandRestart.h"
    "Commands/CommandSetConfig.h"
    "Commands/CommandSFX.h"
    "Commands/CommandSong.h"
    "Commands/CommandSpeakers.h"
    "Commands/CommandSpectate.h"
    "Commands/CommandStrip.h"
    "Commands/CommandSwap.h"
    "Commands/CommandTimer.h"
    "Commands/CommandUnban.h"
    "Commands/CommandUnmod.h"
    "Commands/CommandVideoFix.h"
    "CompilerDirectives.h"
    "Debouncer.h"
    "Dice.h"
    "DualshockGamepad.h"
    "DX11.h"
    "Frequency.h"
    "GamepadClient.h"
    "GamepadState.h"
    "globals/AppColors.h"
    "globals/AppFonts.h"
    "globals/AppIcons.h"
    "globals/AppStyle.h"
    "Guest.h"
    "GuestData.h"
    "GuestDataList.h"
    "GuestDevice.h"
    "GuestList.h"
    "GuestTier.h"
    "Hosting.h"
    "IGamepadReader.h"
    "imgui/imconfig.h"
    "imgui/imgui.h"
    "imgui/imgui_colors.h"
    "imgui/imgui_impl_dx11.h"
    "imgui/imgui_impl_win32.h"
    "imgui/imgui_internal.h"
    "imgui/imstb_rectpack.h"
    "imgui/imstb_textedit.h"
    "imgui/imstb_truetype.h"
    "KeyboardMaps.h"
    "Leaderboard.h"
    "MasterOfPuppets.h"
    "MetadataCache.h"
    "ModList.h"
    "ParsecSession.h"
    "resource.h"
    "SDLGamepad.h"
    "SDLGamepadReader.h"
    "SFXList.h"
    "SpectateList.h"
    "stb_image.h"
    "Stopwatch.h"
    "StressTest.h"
    "Stringer.h"
    "Texture.h"
    "Thumbnail.h"
    "ThumbnailList.h"
    "Tier.h"
    "TierList.h"
    "Timespan.h"
    "TimeT.h"
    "Utils.h"
    "WebSocket.h"
    "Widgets/AlertWidget.h"
    "Widgets/AnimatedGamepadWidget.h"
    "Widgets/AudioControlWidget.h"
    "Widgets/AudioSettingsWidget.h"
    "Widgets/BoolButtonWidget.h"
    "Widgets/ButtonLockWidget.h"
    "Widgets/ChatWidget.h"
    "Widgets/ConfirmPopupWidget.h"
    "Widgets/GamepadsWidget.h"
    "Widgets/GuestListWidget.h"
    "Widgets/HostInfoWidget.h"
    "Widgets/HostSettingsWidget.h"
    "Widgets/IconButton.h"
    "Widgets/InfoWidget.h"
    "Widgets/IntRangeWidget.h"
    "Widgets/LoadingRingWidget.h"
    "Widgets/LoginWidget.h"
    "Widgets/LogWidget.h"
    "Widgets/MasterOfPuppetsWidget.h"
    "Widgets/NavBar.h"
    "Widgets/PopupWidgetEdit.h"
    "Widgets/ProgressCircularWidget.h"
    "Widgets/SettingsWidget.h"
    "Widgets/SmashWidget.h"
    "Widgets/SpotifyWidget.h"
    "Widgets/StylePickerWidget.h"
    "Widgets/ThumbnailsWidget.h"
    "Widgets/TitleTooltipWidget.h"
    "Widgets/ToggleIconButtonWidget.h"
    "Widgets/TooltipWidget.h"
    "Widgets/Vec2.h"
    "Widgets/VersionWidget.h"
    "Widgets/VideoWidget.h"
    "Widgets/VirtualGamepadWidget.h"
    "Widgets/WebSocketWidget.h"
    "XBoxGamepad.h"
    "XInputReader.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "AGamepad.cpp"
    "AudioIn.cpp"
    "AudioMix.cpp"
    "AudioOut.cpp"
    "BanList.cpp"
    "Bitwise.cpp"
    "ChatBot.cpp"
    "ChatLog.cpp"
    "Clock.cpp"
    "Commands/CommandBonk.cpp"
    "Debouncer.cpp"
    "Dice.cpp"
    "DualshockGamepad.cpp"
    "DX11.cpp"
    "Frequency.cpp"
    "GamepadClient.cpp"
    "GamepadState.cpp"
    "globals/AppColors.cpp"
    "globals/AppFonts.cpp"
    "globals/AppIcons.cpp"
    "globals/AppStyle.cpp"
    "Guest.cpp"
    "GuestData.cpp"
    "GuestDataList.cpp"
    "GuestDevice.cpp"
    "GuestList.cpp"
    "Hosting.cpp"
    "imgui/imgui.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_impl_dx11.cpp"
    "imgui/imgui_impl_win32.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "Leaderboard.cpp"
    "Main.cpp"
    "MasterOfPuppets.cpp"
    "MetadataCache.cpp"
    "ModList.cpp"
    "ParsecSession.cpp"
    "SDLGamepad.cpp"
    "SDLGamepadReader.cpp"
    "SFXList.cpp"
    "SpectateList.cpp"
    "Stopwatch.cpp"
    "StressTest.cpp"
    "Stringer.cpp"
    "Texture.cpp"
    "Thumbnail.cpp"
    "ThumbnailList.cpp"
    "TierList.cpp"
    "Timespan.cpp"
    "Utils.cpp"
    "WebSocket.cpp"
    "Widgets/AlertWidget.cpp"
    "Widgets/AnimatedGamepadWidget.cpp"
    "Widgets/AudioControlWidget.cpp"
    "Widgets/AudioSettingsWidget.cpp"
    "Widgets/BoolButtonWidget.cpp"
    "Widgets/ButtonLockWidget.cpp"
    "Widgets/ChatWidget.cpp"
    "Widgets/ConfirmPopupWidget.cpp"
    "Widgets/GamepadsWidget.cpp"
    "Widgets/GuestListWidget.cpp"
    "Widgets/HostInfoWidget.cpp"
    "Widgets/HostSettingsWidget.cpp"
    "Widgets/IconButton.cpp"
    "Widgets/InfoWidget.cpp"
    "Widgets/IntRangeWidget.cpp"
    "Widgets/LoadingRingWidget.cpp"
    "Widgets/LoginWidget.cpp"
    "Widgets/LogWidget.cpp"
    "Widgets/MasterOfPuppetsWidget.cpp"
    "Widgets/NavBar.cpp"
    "Widgets/PopupWidgetEdit.cpp"
    "Widgets/ProgressCircularWidget.cpp"
    "Widgets/SettingsWidget.cpp"
    "Widgets/SmashWidget.cpp"
    "Widgets/SpotifyWidget.cpp"
    "Widgets/StylePickerWidget.cpp"
    "Widgets/ThumbnailsWidget.cpp"
    "Widgets/TitleTooltipWidget.cpp"
    "Widgets/ToggleIconButtonWidget.cpp"
    "Widgets/TooltipWidget.cpp"
    "Widgets/Vec2.cpp"
    "Widgets/VersionWidget.cpp"
    "Widgets/VideoWidget.cpp"
    "Widgets/VirtualGamepadWidget.cpp"
    "Widgets/WebSocketWidget.cpp"
    "XBoxGamepad.cpp"
    "XInputReader.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

add_executable(${PROJECT_NAME} ${ALL_FILES} ${APP_ICON_RESOURCE_WINDOWS})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE ParsecSoda)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
            MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../external;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/parsecsdk;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/matoya;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/vigem/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/sdl/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../external;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/parsecsdk;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/matoya;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/vigem/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../Dependencies/sdl/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /bigobj;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /bigobj;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

################################################################################
# Post build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\fonts\\" "${OUTPUT_DIRECTORY}fonts\\" /E /Y
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\icons\\" "${OUTPUT_DIRECTORY}icons\\" /E /Y
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\sfx\\" "${OUTPUT_DIRECTORY}sfx\\" /E
        COMMAND $<CONFIG:Release> copy /Y "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>Dependencies\\parsecsdk\\windows\\parsec.dll" "${OUTPUT_DIRECTORY}parsec.dll"
        COMMAND $<CONFIG:Release> copy /Y "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>Dependencies\\sdl\\lib\\x64\\SDL2.dll" "${OUTPUT_DIRECTORY}SDL2.dll"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMANDS
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\fonts\\" "${OUTPUT_DIRECTORY}fonts\\" /E /Y
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\icons\\" "${OUTPUT_DIRECTORY}icons\\" /E /Y
        COMMAND $<CONFIG:Release> xcopy "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>ParsecSoda\\sfx\\" "${OUTPUT_DIRECTORY}sfx\\" /E
        COMMAND $<CONFIG:Release> copy /Y "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>Dependencies\\parsecsdk\\windows\\parsec32.dll" "${OUTPUT_DIRECTORY}parsec32.dll"
        COMMAND $<CONFIG:Release> copy /Y "$<SHELL_PATH:${CMAKE_SOURCE_DIR}/>Dependencies\\sdl\\lib\\x86\\SDL2.dll" "${OUTPUT_DIRECTORY}SDL2.dll"
    )
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "libucrtd"
        ">"
        "parsec;"
        "libucrt;"
        "matoya;"
        "Winhttp;"
        "Crypt32;"
        "Secur32;"
        "libvcruntime;"
        "libcmt;"
        "kernel32;"
        "gdi32;"
        "winmm;"
        "imm32;"
        "shell32;"
        "advapi32;"
        "ole32;"
        "oleaut32;"
        "opengl32;"
        "user32;"
        "uuid;"
        "version;"
        "setupapi;"
        "hid;"
        "dxgi;"
        "ws2_32;"
        "bcrypt;"
        "windowscodecs;"
        "xinput9_1_0;"
        "d3d9;"
        "shlwapi;"
        "d3d11;"
        "ViGEmClient;"
        "SDL2;"
        "SDL2main"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "libucrtd"
        ">"
        "parsec32;"
        "libucrt;"
        "matoya;"
        "Winhttp;"
        "Crypt32;"
        "Secur32;"
        "libvcruntime;"
        "libcmt;"
        "kernel32;"
        "gdi32;"
        "winmm;"
        "imm32;"
        "shell32;"
        "advapi32;"
        "ole32;"
        "oleaut32;"
        "opengl32;"
        "user32;"
        "uuid;"
        "version;"
        "setupapi;"
        "hid;"
        "dxgi;"
        "ws2_32;"
        "bcrypt;"
        "windowscodecs;"
        "xinput9_1_0;"
        "d3d9;"
        "shlwapi;"
        "d3d11;"
        "ViGEmClient;"
        "SDL2;"
        "SDL2main"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_SOURCE_DIR}/Dependencies/vigem/lib/debug/x64"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_SOURCE_DIR}/Dependencies/vigem/lib/release/x64"
        ">"
        "${CMAKE_SOURCE_DIR}/Dependencies/parsecsdk/windows;"
        "${CMAKE_SOURCE_DIR}/Dependencies/matoya/64;"
        "${CMAKE_SOURCE_DIR}/Dependencies/sdl/lib/x64"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_SOURCE_DIR}/Dependencies/vigem/lib/debug/x86"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_SOURCE_DIR}/Dependencies/vigem/bin/release32"
        ">"
        "${CMAKE_SOURCE_DIR}/Dependencies/parsecsdk/windows;"
        "${CMAKE_SOURCE_DIR}/Dependencies/matoya/32;"
        "${CMAKE_SOURCE_DIR}/Dependencies/sdl/lib/x86"
    )
endif()

