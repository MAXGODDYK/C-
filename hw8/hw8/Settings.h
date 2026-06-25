#pragma once

// ТЕМА 8 (ДЗ). Налаштування застосунку (аналог Properties.Settings із C#).
// Зберігає між запусками: останній каталог файлів (LastPath) та розмір і
// позицію головного вікна (WindowLocation/WindowSize).
//
// Реалізовано вручну на базі ApplicationSettingsBase, бо в проєкті C++/CLI немає
// дизайнера .settings. Значення зберігаються у файл user.config. Потрібне
// посилання на System.Configuration (додано у .vcxproj).

namespace Portfolio {
    namespace Properties {

        using namespace System;
        using namespace System::Drawing;
        using namespace System::Configuration;

        public ref class Settings : public ApplicationSettingsBase {
        private:
            static Settings^ defaultInstance =
                safe_cast<Settings^>(ApplicationSettingsBase::Synchronized(gcnew Settings()));

        public:
            static property Settings^ Default {
                Settings^ get() { return defaultInstance; }
            }

            // Останній каталог збереження/завантаження.
            [UserScopedSetting]
            [DefaultSettingValue("")]
            property String^ LastPath {
                String^ get() { return safe_cast<String^>(this["LastPath"]); }
                void set(String^ value) { this["LastPath"] = value; }
            }

            // Позиція вікна (за замовчуванням 0,0 = «не збережено»).
            [UserScopedSetting]
            [DefaultSettingValue("0, 0")]
            property System::Drawing::Point WindowLocation {
                System::Drawing::Point get() { return safe_cast<System::Drawing::Point>(this["WindowLocation"]); }
                void set(System::Drawing::Point value) { this["WindowLocation"] = value; }
            }

            // Розмір вікна (за замовчуванням 0,0 = «не збережено» -> CenterScreen).
            [UserScopedSetting]
            [DefaultSettingValue("0, 0")]
            property System::Drawing::Size WindowSize {
                System::Drawing::Size get() { return safe_cast<System::Drawing::Size>(this["WindowSize"]); }
                void set(System::Drawing::Size value) { this["WindowSize"] = value; }
            }
        };
    }
}
