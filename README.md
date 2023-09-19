# weather-panel
### Monitor weather activity
>#### https://open-meteo.com/
## Building
#### This repository uses github [submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to include external code in the project.
#### The [solution](./src/weather-panel.sln) can be built with [MSBuild](https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-command-line-reference) by running the following command in a [Developer Command Prompt or Developer PowerShell](https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell) in the [`src`](./src/) directory asfter all dependancies have been downloaded.
```ps1
msbuild weather-panel.sln /p:Configuration=Release /p:Platform=x64
```
>###### copyright &copy; 2023 aroary
