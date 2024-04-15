Remove-Item -Recurse -Force .\x64\Release\** -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force .\x86\Release\** -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force .\TqkLibrary.AudioPlayer.XAudio2\bin\Release\** -ErrorAction SilentlyContinue

$env:PATH="$($env:PATH);C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE"
devenv .\TqkLibrary.AudioPlayer.XAudio2.sln /Rebuild 'Release|x64' /Project TqkLibrary.AudioPlayer.XAudio2.Native
devenv .\TqkLibrary.AudioPlayer.XAudio2.sln /Rebuild 'Release|x86' /Project TqkLibrary.AudioPlayer.XAudio2.Native

dotnet build --no-incremental .\TqkLibrary.AudioPlayer.XAudio2\TqkLibrary.AudioPlayer.XAudio2.csproj -c Release
nuget pack .\TqkLibrary.AudioPlayer.XAudio2\TqkLibrary.AudioPlayer.XAudio2.nuspec -Symbols -OutputDirectory .\TqkLibrary.AudioPlayer.XAudio2\bin\Release

$localNuget = $env:localNuget
if(![string]::IsNullOrWhiteSpace($localNuget))
{
    Copy-Item .\TqkLibrary.AudioPlayer.XAudio2\bin\Release\*.nupkg -Destination $localNuget -Force
}

