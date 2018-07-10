//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "GraphicsGL.h"

#include "../Configuration.h"
#include "../Console.h"
#include "../IO/Window.h"

#include <algorithm>

namespace jrc
{
Rectangle<std::int16_t> GraphicsGL::screen;

GraphicsGL::GraphicsGL()
{
    screen = {0,
              Constants::VIEW_WIDTH,
              -Constants::VIEW_Y_OFFSET,
              -Constants::VIEW_Y_OFFSET + Constants::VIEW_HEIGHT};
    locked = false;
}

Error GraphicsGL::init()
{
    if (glewInit()) {
        return Error::GLEW;
    }

    if (FT_Init_FreeType(&ft_library)) {
        return Error::FREETYPE;
    }

    GLint result = GL_FALSE;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vs_source
        = "#version 120\n"
          "attribute vec4 coord;"
          "attribute vec4 color;"
          "varying vec2 texpos;"
          "varying vec4 colormod;"
          "uniform vec2 screensize;"
          "uniform int yoffset;"

          "void main(void) {"
          "    float x = -1.0 + coord.x * 2.0 / screensize.x;"
          "    float y = 1.0 - (coord.y + yoffset) * 2.0 / screensize.y;"
          "    gl_Position = vec4(x, y, 0.0, 1.0);"
          "    texpos = coord.zw;"
          "    colormod = color;"
          "}";
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
    if (!result) {
        return Error::VERTEX_SHADER;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs_source
        = "#version 120\n"
          "varying vec2 texpos;"
          "varying vec4 colormod;"
          "uniform sampler2D texture;"
          "uniform vec2 atlassize;"
          "uniform int fontregion;"

          "void main(void) {"
          "    if (texpos.y == 0) {"
          "        gl_FragColor = colormod;"
          "    } else if (texpos.y <= fontregion) {"
          "        gl_FragColor = vec4(1, 1, 1, texture2D(texture, texpos / "
          "atlassize).r) * colormod;"
          "    } else {"
          "        gl_FragColor = texture2D(texture, texpos / atlassize) * "
          "colormod;"
          "    }"
          "}";
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
    if (!result) {
        return Error::FRAGMENT_SHADER;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result) {
        return Error::SHADER_PROGRAM;
    }

    attribute_coord = glGetAttribLocation(program, "coord");
    attribute_color = glGetAttribLocation(program, "color");
    uniform_texture = glGetUniformLocation(program, "texture");
    uniform_atlas_size = glGetUniformLocation(program, "atlassize");
    uniform_screen_size = glGetUniformLocation(program, "screensize");
    uniform_y_offset = glGetUniformLocation(program, "yoffset");
    uniform_font_region = glGetUniformLocation(program, "fontregion");
    if (attribute_coord == -1 || attribute_color == -1 || uniform_texture == -1
        || uniform_atlas_size == -1 || uniform_y_offset == -1
        || uniform_screen_size == -1) {
        return Error::SHADER_VARS;
    }

    glGenBuffers(1, &vbo);

    glGenTextures(1, &atlas);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 ATLASW,
                 ATLASH,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 nullptr);

    font_border.set_y(1);

    const std::string& FONT_NORMAL = Configuration::get().fonts.normal;
    const std::string& FONT_BOLD = Configuration::get().fonts.bold;
    if (FONT_NORMAL.empty() || FONT_BOLD.empty()) {
        Console::get().print(
            "[Warning] A font path is empty, check your settings file.");
    }

    const char* const FONT_NORMAL_STR = FONT_NORMAL.data();
    const char* const FONT_BOLD_STR = FONT_BOLD.data();

    addfont(FONT_NORMAL_STR, Text::A11L, 0, 11);
    addfont(FONT_NORMAL_STR, Text::A11M, 0, 11);
    addfont(FONT_BOLD_STR, Text::A11B, 0, 11);
    addfont(FONT_NORMAL_STR, Text::A12M, 0, 12);
    addfont(FONT_BOLD_STR, Text::A12B, 0, 12);
    addfont(FONT_NORMAL_STR, Text::A13M, 0, 13);
    addfont(FONT_BOLD_STR, Text::A13B, 0, 13);
    addfont(FONT_NORMAL_STR, Text::A18M, 0, 18);

    font_y_max += font_border.y();

    leftovers = QuadTree<std::size_t, Leftover>{
        [](const Leftover& first, const Leftover& second) {
            bool wcomp = first.width() >= second.width();
            bool hcomp = first.height() >= second.height();
            if (wcomp && hcomp) {
                return QuadTree<std::size_t, Leftover>::RIGHT;
            } else if (wcomp) {
                return QuadTree<std::size_t, Leftover>::DOWN;
            } else if (hcomp) {
                return QuadTree<std::size_t, Leftover>::UP;
            } else {
                return QuadTree<std::size_t, Leftover>::LEFT;
            }
        }};

    return Error::NONE;
}

bool GraphicsGL::addfont(const char* name,
                         Text::Font id,
                         FT_UInt pixelw,
                         FT_UInt pixelh)
{
    FT_Face face;
    if (FT_New_Face(ft_library, name, 0, &face)) {
        return false;
    }

    if (FT_Set_Pixel_Sizes(face, pixelw, pixelh)) {
        return false;
    }

    FT_GlyphSlot g = face->glyph;

    GLshort width = 0;
    GLshort height = 0;
    for (std::uint8_t c = 32; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        GLshort w = static_cast<GLshort>(g->bitmap.width);
        GLshort h = static_cast<GLshort>(g->bitmap.rows);

        width += w;
        if (h > height) {
            height = h;
        }
    }

    if (font_border.x() + width > ATLASW) {
        font_border.set_x(0);
        font_border.set_y(font_y_max);
        font_y_max = 0;
    }

    GLshort x = font_border.x();
    GLshort y = font_border.y();

    font_border.shift_x(width);
    if (height > font_y_max) {
        font_y_max = height;
    }

    fonts[id] = Font(width, height);

    GLshort ox = x;
    GLshort oy = y;
    for (std::uint8_t c = 32; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        GLshort ax = static_cast<GLshort>(g->advance.x >> 6);
        GLshort ay = static_cast<GLshort>(g->advance.y >> 6);
        GLshort l = static_cast<GLshort>(g->bitmap_left);
        GLshort t = static_cast<GLshort>(g->bitmap_top);
        GLshort w = static_cast<GLshort>(g->bitmap.width);
        GLshort h = static_cast<GLshort>(g->bitmap.rows);

        glTexSubImage2D(GL_TEXTURE_2D,
                        0,
                        ox,
                        oy,
                        w,
                        h,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        g->bitmap.buffer);

        Offset offset = Offset(ox, oy, w, h);
        fonts[id].chars[c] = {ax, ay, w, h, l, t, offset};

        ox += w;
    }

    return true;
}

void GraphicsGL::reinit()
{
    glUseProgram(program);

    glUniform1i(uniform_y_offset, Constants::VIEW_Y_OFFSET);
    glUniform1i(uniform_font_region, font_y_max);
    glUniform2f(uniform_atlas_size, ATLASW, ATLASH);
    glUniform2f(uniform_screen_size,
                Window::get().get_width(),
                Window::get().get_height());

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(
        attribute_coord, 4, GL_SHORT, GL_FALSE, sizeof(Quad::Vertex), 0);
    glVertexAttribPointer(attribute_color,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Quad::Vertex),
                          (const void*)8);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, atlas);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    clear_internal();
}

void GraphicsGL::clear_internal()
{
    border = Point<GLshort>(0, font_y_max);
    y_range = Range<GLshort>();

    offsets.clear();
    leftovers.clear();
    rlid = 1;
    wasted = 0;
}

void GraphicsGL::clear()
{
    std::size_t used = ATLASW * border.y() + border.x() * y_range.second();
    double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
    if (usedpercent > 80.0) {
        clear_internal();
    }
}

void GraphicsGL::add_bitmap(const nl::bitmap& bmp)
{
    get_offset(bmp);
}

const GraphicsGL::Offset& GraphicsGL::get_offset(const nl::bitmap& bmp)
{
    std::size_t id = bmp.id();
    auto offiter = offsets.find(id);
    if (offiter != offsets.end()) {
        return offiter->second;
    }

    GLshort x = 0;
    GLshort y = 0;
    GLshort w = bmp.width();
    GLshort h = bmp.height();

    if (w <= 0 || h <= 0) {
        return null_offset;
    }

    const auto bmp_data = bmp.data();
    if (!bmp_data) {
        return null_offset;
    }

    auto value = Leftover(x, y, w, h);
    std::size_t lid = leftovers.find_node(value, [
    ](const Leftover& val, const Leftover& leaf) noexcept {
        return val.width() <= leaf.width() && val.height() <= leaf.height();
    });

    if (lid > 0) {
        const Leftover& leftover = leftovers[lid];

        x = leftover.l;
        y = leftover.t;

        GLshort wdelta = leftover.width() - w;
        GLshort hdelta = leftover.height() - h;

        leftovers.erase(lid);

        wasted -= w * h;

        if (wdelta >= MINLOSIZE && hdelta >= MINLOSIZE) {
            leftovers.add(rlid, Leftover(x + w, y + h, wdelta, hdelta));
            ++rlid;

            if (w >= MINLOSIZE) {
                leftovers.add(rlid, Leftover(x, y + h, w, hdelta));
                ++rlid;
            }

            if (h >= MINLOSIZE) {
                leftovers.add(rlid, Leftover(x + w, y, wdelta, h));
                ++rlid;
            }
        } else if (wdelta >= MINLOSIZE) {
            leftovers.add(rlid, Leftover(x + w, y, wdelta, h + hdelta));
            ++rlid;
        } else if (hdelta >= MINLOSIZE) {
            leftovers.add(rlid, Leftover(x, y + h, w + wdelta, hdelta));
            ++rlid;
        }
    } else {
        if (border.x() + w > ATLASW) {
            border.set_x(0);
            border.shift_y(y_range.second());
            if (border.y() + h > ATLASH) {
                clear_internal();
            } else {
                y_range = Range<GLshort>();
            }
        }
        x = border.x();
        y = border.y();

        border.shift_x(w);

        if (h > y_range.second()) {
            if (x >= MINLOSIZE && h - y_range.second() >= MINLOSIZE) {
                leftovers.add(
                    rlid,
                    Leftover(0, y_range.first(), x, h - y_range.second()));
                ++rlid;
            }

            wasted += x * (h - y_range.second());

            y_range = {y + h, h};
        } else if (h < y_range.first() - y) {
            if (w >= MINLOSIZE && y_range.first() - y - h >= MINLOSIZE) {
                leftovers.add(rlid,
                              Leftover(x, y + h, w, y_range.first() - y - h));
                ++rlid;
            }

            wasted += w * (y_range.first() - y - h);
        }
    }

    /*
    std::size_t used = ATLASW * border.y() + border.x() * y_range.second();
    double usedpercent = static_cast<double>(used) / (ATLASW * ATLASH);
    double wastedpercent = static_cast<double>(wasted) / used;
    Console::get().print("Used: " + std::to_string(usedpercent) + ", wasted: "
    + std::to_string(wastedpercent));
    */

    glTexSubImage2D(
        GL_TEXTURE_2D, 0, x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, bmp_data);

    return offsets
        .emplace(std::piecewise_construct,
                 std::forward_as_tuple(id),
                 std::forward_as_tuple(x, y, w, h))
        .first->second;
}

void GraphicsGL::draw(const nl::bitmap& bmp,
                      const Rectangle<std::int16_t>& rect,
                      const Color& color,
                      float angle)
{
    if (locked) {
        return;
    }

    if (color.invisible()) {
        return;
    }

    if (!rect.overlaps(screen)) {
        return;
    }

    quads.emplace_back(
        rect.l(), rect.r(), rect.t(), rect.b(), get_offset(bmp), color, angle);
}

Text::Layout GraphicsGL::create_layout(std::string_view text,
                                       Text::Font id,
                                       Text::Alignment alignment,
                                       std::int16_t max_width,
                                       bool formatted)
{
    auto length = text.length();
    if (length == 0) {
        return {};
    }

    LayoutBuilder builder{fonts[id], alignment, max_width, formatted};

    std::string_view::size_type first = 0;
    std::string_view::size_type offset = 0;
    while (offset < length) {
        auto last = text.find_first_of(" \\#", offset + 1, 3);
        if (last == std::string_view::npos) {
            last = length;
        }

        first = builder.add(text, first, offset, last);
        offset = last;
    }

    return builder.finish(first, offset);
}

GraphicsGL::LayoutBuilder::LayoutBuilder(const Font& f,
                                         Text::Alignment a,
                                         std::int16_t mw,
                                         bool fm)
    : font(f), alignment(a), max_width(mw), formatted(fm)
{
    font_id = Text::NUM_FONTS;
    color = Text::NUM_COLORS;
    ax = 0;
    ay = font.line_space();
    width = 0;
    endy = 0;
    if (max_width == 0) {
        max_width = Window::get().get_width();
    }
}

std::string_view::size_type
GraphicsGL::LayoutBuilder::add(std::string_view text,
                               std::string_view::size_type prev,
                               std::string_view::size_type first,
                               std::string_view::size_type last)
{
    if (first == last) {
        return prev;
    }

    Text::Font last_font = font_id;
    Text::Color last_color = color;
    std::string_view::size_type skip = 0;
    bool line_break = false;
    if (formatted) {
        switch (text[first]) {
        case '\\':
            if (first + 1 < last) {
                switch (text[first + 1]) {
                case 'n':
                    line_break = true;
                    break;
                case 'r':
                    line_break = ax > 0;
                    break;
                }
                ++skip;
            }
            ++skip;
            break;
        case '#':
            if (first + 1 < last) {
                switch (text[first + 1]) {
                case 'k':
                    color = Text::DARKGREY;
                    break;
                case 'b':
                    color = Text::BLUE;
                    break;
                case 'r':
                    color = Text::RED;
                    break;
                case 'c':
                    color = Text::ORANGE;
                    break;
                }
                ++skip;
            }
            ++skip;
            break;
        }
    }

    std::int16_t word_width = 0;
    if (!line_break) {
        for (std::string_view::size_type i = first; i < last; ++i) {
            char c = text[i];
            word_width += font.chars[static_cast<std::size_t>(c)].ax;

            if (word_width > max_width) {
                if (last - first == 1) {
                    return last;
                } else {
                    prev = add(text, prev, first, i);
                    return add(text, prev, i, last);
                }
            }
        }
    }

    bool new_word = skip > 0;
    bool new_line = line_break || ax + word_width > max_width;
    if (new_word || new_line) {
        add_word(prev, first, last_font, last_color);
    }
    if (new_line) {
        add_line();

        endy = ay;
        ax = 0;
        ay += font.line_space();
    }

    for (std::string_view::size_type pos = first; pos < last; ++pos) {
        char c = text[pos];
        const Font::Char& ch = font.chars[static_cast<std::size_t>(c)];

        advances.push_back(ax);

        if (pos < first + skip || (new_line && c == ' ')) {
            continue;
        }

        ax += ch.ax;

        if (width < ax) {
            width = ax;
        }
    }

    if (new_word || new_line) {
        return first + skip;
    } else {
        return prev;
    }
}

Text::Layout GraphicsGL::LayoutBuilder::finish(std::size_t first,
                                               std::size_t last)
{
    add_word(first, last, font_id, color);
    add_line();

    advances.push_back(ax);
    return {lines, advances, width, ay, ax, endy};
}

void GraphicsGL::LayoutBuilder::add_word(std::size_t word_first,
                                         std::size_t word_last,
                                         Text::Font word_font,
                                         Text::Color word_color)
{
    words.push_back({word_first, word_last, word_font, word_color});
}

void GraphicsGL::LayoutBuilder::add_line()
{
    std::int16_t line_x = 0;
    std::int16_t line_y = ay;
    switch (alignment) {
    case Text::CENTER:
        line_x -= ax / 2;
        break;
    case Text::RIGHT:
        line_x -= ax;
        break;
    default:
        break;
    }

    lines.push_back({words, {line_x, line_y}});
    words.clear();
}

void GraphicsGL::draw_text(const DrawArgument& args,
                           std::string_view text,
                           const Text::Layout& layout,
                           Text::Font id,
                           Text::Color colorid,
                           Text::Background background)
{
    if (locked) {
        return;
    }

    const Color& color = args.get_color();

    if (text.empty() || color.invisible()) {
        return;
    }

    const Font& font = fonts[id];

    GLshort x = args.getpos().x();
    GLshort y = args.getpos().y();
    GLshort w = layout.width();
    GLshort h = layout.height();

    switch (background) {
    case Text::NAMETAG:
        for (const Text::Layout::Line& line : layout) {
            GLshort left = x + line.position.x() - 2;
            GLshort right = left + w + 3;
            GLshort top = y + line.position.y() - font.line_space() + 5;
            GLshort bottom = top + h - 2;
            Color ntcolor{0.0f, 0.0f, 0.0f, 0.6f};

            quads.emplace_back(
                left, right, top, bottom, null_offset, ntcolor, 0.0f);
            quads.emplace_back(left - 1,
                               left,
                               top + 1,
                               bottom - 1,
                               null_offset,
                               ntcolor,
                               0.0f);
            quads.emplace_back(right,
                               right + 1,
                               top + 1,
                               bottom - 1,
                               null_offset,
                               ntcolor,
                               0.0f);
        }
        break;
    default:
        break;
    }

    static constexpr const GLfloat colors[Text::NUM_COLORS][3] = {
        {0.0f, 0.0f, 0.0f},    // Black
        {1.0f, 1.0f, 1.0f},    // White
        {1.0f, 1.0f, 0.0f},    // Yellow
        {0.0f, 0.0f, 1.0f},    // Blue
        {1.0f, 0.0f, 0.0f},    // Red
        {0.8f, 0.3f, 0.3f},    // DarkRed
        {0.5f, 0.25f, 0.0f},   // Brown
        {0.5f, 0.5f, 0.5f},    // Lightgrey
        {0.25f, 0.25f, 0.25f}, // Darkgrey
        {1.0f, 0.5f, 0.0f},    // Orange
        {0.0f, 0.75f, 1.0f},   // Mediumblue
        {0.5f, 0.0f, 0.5f}     // Violet
    };

    for (const Text::Layout::Line& line : layout) {
        Point<std::int16_t> position = line.position;

        for (const Text::Layout::Word& word : line.words) {
            GLshort ax = position.x() + layout.advance(word.first);
            GLshort ay = position.y();

            const GLfloat* wordcolor;
            if (word.color < Text::NUM_COLORS) {
                wordcolor = colors[word.color];
            } else {
                wordcolor = colors[colorid];
            }
            Color abscolor
                = color
                  * Color{wordcolor[0], wordcolor[1], wordcolor[2], 1.0f};

            for (std::size_t pos = word.first; pos < word.last; ++pos) {
                const char c = text[pos];
                const Font::Char& ch = font.chars[static_cast<std::size_t>(c)];

                GLshort chx = x + ax + ch.bl;
                GLshort chy = y + ay - ch.bt;
                GLshort chw = ch.bw;
                GLshort chh = ch.bh;

                if (ax == 0 && c == ' ') {
                    continue;
                }

                ax += ch.ax;

                if (chw <= 0 || chh <= 0) {
                    continue;
                }

                quads.emplace_back(
                    chx, chx + chw, chy, chy + chh, ch.offset, abscolor, 0.0f);
            }
        }
    }
}

void GraphicsGL::draw_rectangle(std::int16_t x,
                                std::int16_t y,
                                std::int16_t w,
                                std::int16_t h,
                                float r,
                                float g,
                                float b,
                                float a)
{
    if (locked) {
        return;
    }

    quads.emplace_back(
        x, x + w, y, y + h, null_offset, Color{r, g, b, a}, 0.0f);
}

void GraphicsGL::draw_screen_fill(float r, float g, float b, float a)
{
    draw_rectangle(0,
                   -Constants::VIEW_Y_OFFSET,
                   Window::get().get_width(),
                   Window::get().get_height(),
                   r,
                   g,
                   b,
                   a);
}

void GraphicsGL::lock()
{
    locked = true;
}

void GraphicsGL::unlock()
{
    locked = false;
}

void GraphicsGL::flush(float opacity)
{
    bool coverscene = opacity != 1.0f;
    if (coverscene) {
        float complement = 1.0f - opacity;
        Color color{0.0f, 0.0f, 0.0f, complement};

        quads.emplace_back(screen.l(),
                           screen.r(),
                           screen.t(),
                           screen.b(),
                           null_offset,
                           color,
                           0.0f);
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLsizei csize = static_cast<GLsizei>(quads.size() * sizeof(Quad));
    GLsizei fsize = static_cast<GLsizei>(quads.size() * Quad::LENGTH);
    glEnableVertexAttribArray(attribute_coord);
    glEnableVertexAttribArray(attribute_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, csize, quads.data(), GL_STREAM_DRAW);
    glDrawArrays(GL_QUADS, 0, fsize);

    glDisableVertexAttribArray(attribute_coord);
    glDisableVertexAttribArray(attribute_color);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (coverscene) {
        quads.pop_back();
    }
}

void GraphicsGL::clearscene()
{
    if (!locked) {
        quads.clear();
    }
}

void GraphicsGL::set_screen(Rectangle<std::int16_t>&& new_screen) noexcept
{
    screen = new_screen;
}

void GraphicsGL::set_screen(std::int16_t l,
                            std::int16_t r,
                            std::int16_t t,
                            std::int16_t b) noexcept
{
    screen = {l, r, t, b};
}
} // namespace jrc
