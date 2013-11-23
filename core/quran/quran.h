#ifndef QURAN_HH
#define QURAN_HH

#include <vector>
#include <string>
#include "core/quran/verse.h"
#include "core/quran/chapter.h"

namespace quran {
class Quran {
public:
    enum class TextType {
        Arabic, English
    };
    Quran(void);
    void load(const TextType& textType);
    const quran::Verse* verse(Chapter::Name chapter, int verseNumber) const;
    const quran::Chapters& chapters(void) const;
    const quran::Chapter* chapter(Chapter::Name chapter) const;
    bool ready(void) const;
    TextType textType(void) const;
private:
    quran::Chapters m_chapters;
    bool m_ready;
    TextType m_textType;
};
} // namespace quran
#endif // QURAN_HH