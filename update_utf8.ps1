$vcxprojPath = "C:\C-\sr3-Тема5\sr3\StudentPortfolio.vcxproj"
$content = [System.IO.File]::ReadAllText($vcxprojPath, [System.Text.Encoding]::UTF8)

# Add /utf-8 to Debug|Win32
$content = $content -replace '(<BasicRuntimeChecks>Default</BasicRuntimeChecks>)([\s]*</ClCompile>)', '$1`n      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>$2'

# Add /utf-8 to Release|Win32 (after LanguageStandard)
$content = $content -replace '(Release\|Win32.*?<LanguageStandard>stdcpp17</LanguageStandard>)([\s]*</ClCompile>)', '$1`n      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>$2'

# Add /utf-8 to Debug|x64 (after BasicRuntimeChecks)
$content = $content -replace '(Debug\|x64.*?<BasicRuntimeChecks>Default</BasicRuntimeChecks>)([\s]*</ClCompile>)', '$1`n      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>$2'

# Add /utf-8 to Release|x64 (after LanguageStandard)
$content = $content -replace '(Release\|x64.*?<LanguageStandard>stdcpp17</LanguageStandard>)([\s]*</ClCompile>)', '$1`n      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>$2'

[System.IO.File]::WriteAllText($vcxprojPath, $content, [System.Text.Encoding]::UTF8)
Write-Host "Updated StudentPortfolio.vcxproj with /utf-8 flag"
