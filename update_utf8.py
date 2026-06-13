import re

path = r'C:\C-\sr3-Тема5\sr3\StudentPortfolio.vcxproj'
with open(path, 'r', encoding='utf-8') as f:
	content = f.read()

# Check if already has /utf-8 in AdditionalOptions
if '/utf-8' in content:
	print('Already has /utf-8 flag')
else:
	# Add before each </ClCompile> (but not duplicate)
	pattern = r'(      <LanguageStandard>stdcpp17</LanguageStandard>)\n(    </ClCompile>)'
	replacement = r'\1\n      <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>\n\2'
	new_content = re.sub(pattern, replacement, content)

	with open(path, 'w', encoding='utf-8') as f:
		f.write(new_content)
	print('Updated StudentPortfolio.vcxproj with /utf-8')
