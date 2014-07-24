/*
**  VidThumb - Video Thumbnailer
**  Copyright (C) 2014 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "directory_thumbnailer.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/format.hpp>
#include <logmich.hpp>

DirectoryThumbnailer::DirectoryThumbnailer(int num) :
  m_num(num),
  m_thumbnails()
{
}

std::vector<gint64>
DirectoryThumbnailer::get_thumbnail_pos(gint64 duration)
{
  int n = m_num;
  std::vector<gint64> lst;
  for(int i = 0; i < n; ++i)
  {
    lst.push_back(duration/n/2 + duration/n * i);
  }
  return lst;
}

void
DirectoryThumbnailer::receive_frame(Cairo::RefPtr<Cairo::ImageSurface> img, gint64 pos)
{
  m_thumbnails.push_back({img, pos});
}

void
DirectoryThumbnailer::save(const std::string& directory_str)
{
  boost::filesystem::path directory(directory_str);
  
  if (!boost::filesystem::is_directory(directory))
  {
    boost::filesystem::create_directory(directory);
  }

  for(auto& thumb : m_thumbnails)
  {
    boost::filesystem::path filename = directory / (boost::format("thumb%020d.png") % thumb.pos).str();

    log_info("writing thumbnail to %s", filename);
    thumb.image->write_to_png(filename.string());
  }
}

/* EOF */
