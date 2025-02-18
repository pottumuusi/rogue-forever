# How to enable powershell scripts
# 1. Run powershell as administrator
# 2. Run 'set-executionpolicy remotesigned'

$install_assets = 'TRUE'
$install_maps = 'TRUE'
$install_sdl2 = 'TRUE'

$sdl2_dir = 'SDL2-2.30.6-win32-x64'
$sdl2_zip = 'SDL2-2.30.6-win32-x64.zip'
$sdl2_image_dir = 'SDL2_image-2.8.2-win32-x64'
$sdl2_image_zip = 'SDL2_image-2.8.2-win32-x64.zip'
$assets_zip = 'assets_basic.zip'
$maps_zip = 'maps_basic.zip'

$download_destination = $PSScriptRoot

if ("TRUE" -ceq "$install_assets") {
    Write-Output 'Installing assets'

    Invoke-WebRequest \
        -Uri http://gitlab.justworks.today/rogue-forever/$assets_zip
        -OutFile $download_destination\$assets_zip

    Expand-Archive $assets_basic -DestinationPath .\
}

if ("TRUE" -ceq "$install_maps") {
    Write-Output 'Installing maps'

    Invoke-WebRequest \
        -Uri http://gitlab.justworks.today/rogue-forever/$maps_zip
        -OutFile $download_destination\$maps_zip

    Expand-Archive $maps_zip -DestinationPath .\
}

if ("TRUE" -ceq "$install_sdl2") {
    Write-Output 'Installing SDL2'

    Invoke-WebRequest \
        -Uri https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/$sdl2_zip \
        -OutFile $download_destination\$sdl2_zip
    Invoke-WebRequest \
        -Uri https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/$sdl2_image_zip \
        -OutFile $download_destination\$sdl2_zip

    Expand-Archive $sdl2_zip -DestinationPath $sdl2_dir
    Expand-Archive $sdl2_image_zip -DestinationPath $sdl2_image_dir

    Move-Item $sdl2_dir\SDL2.dll .\
    Move-Item $sdl2_dir\SDL2_image.dll .\
}
