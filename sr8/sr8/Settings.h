#pragma once

// ТЕМА 8. Налаштування застосунку (аналог Properties.Settings із C#).
// Зберігає між запусками останній каталог збереження/відкриття файлу (LastPath).
//
// Реалізовано вручну на базі ApplicationSettingsBase, бо в проєкті C++/CLI немає
// дизайнера .settings. Значення зберігаються у файл user.config, який створює
// провайдер налаштувань за замовчуванням. Потрібне посилання на
// System.Configuration (додано у .vcxproj).

namespace Portfolio {
    namespace Properties {

        using namespace System;
        using namespace System::Configuration;

        public ref class Settings : public ApplicationSettingsBase {
        private:
            // Єдиний потокобезпечний екземпляр налаштувань.
            static Settings^ defaultInstance =
                safe_cast<Settings^>(ApplicationSettingsBase::Synchronized(gcnew Settings()));

        public:
            // Доступ за зразком C#: Properties::Settings::Default->LastPath.
            static property Settings^ Default {
                Settings^ get() { return defaultInstance; }
            }

            // Останній каталог збереження/завантаження. За замовчуванням порожній.
            [UserScopedSetting]
            [DefaultSettingValue("")]
            property String^ LastPath {
                String^ get() { return safe_cast<String^>(this["LastPath"]); }
                void set(String^ value) { this["LastPath"] = value; }
            }
        };
    }
}
