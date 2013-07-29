require 'chunky_png'
require 'ruby-progressbar'

module IMG

  class Converter

    def img2screen(image)
      screen = IMG::ScreenFormat.new();
      (0..(EPD27::PAGES_PER_SCREEN-1)).each {|page_idx| 
        page = IMG::PageFormat.new()
        (0..(EPD27::LINES_PER_PAGE-1)).each {|line_idx|
          current_row = page_idx * EPD27::LINES_PER_PAGE + line_idx
          if (current_row < EPD27::LINES_PER_DISPLAY)
            puts "Page #{page_idx}, line #{line_idx}, on image row #{current_row}" if $verbose
            page.lines[line_idx] = imgrow2line(image.row(current_row));
          else
            puts "Inserting blank line for padding." if $verbose
            page.lines[line_idx] = IMG::LineFormat.new();
          end
        }
        screen.pages[page_idx] = page;
      }
      return screen;
    end

    def imgrow2line(row)
      line=IMG::LineFormat.new()
      #puts "Got row with length #{row.length}: #{row}"
      if (row.length != EPD27::PIXEL_PER_LINE)
        raise "Invalid row size in row2line converter: #{row.length}"
      end
      (0..(EPD27::BYTES_PER_LINE-1)).each{|byte_idx|
        curbyte = 0x00;
        ((byte_idx*8)..((byte_idx+1)*8)-1).each{ |pixel_idx|
          subidx = pixel_idx.modulo(8);
          pixel = row[byte_idx*8 + subidx]
          if pixel == ChunkyPNG::Color::BLACK # black pixel
            curbyte = curbyte | (1 << subidx)
            puts "pixel_idx: #{pixel_idx}, byte_idx: #{byte_idx}, subidx #{subidx}," \
            << " pixel index: #{byte_idx*8+subidx}, pixel value: #{pixel}, black" if $verbose
          else # white pixel
            curbyte = curbyte & ~(1 << pixel_idx.modulo(8))
            puts "pixel_idx: #{pixel_idx}, byte_idx: #{byte_idx}, subidx #{subidx}," \
            << " pixel index: #{byte_idx*8+subidx}, pixel value: #{pixel}, white" if $verbose
          end
        }
        line[byte_idx]=curbyte;
      }
      return line
    end

    def munch_directory(basedir)

      num_screens=0;
      for file in Dir.entries(basedir)
        if file =~ /^img-(\d+)-(\d+).png/ 
          num_screens+=1
        end
      end
      
      flash = IMG::FlashFormat.new()
      lut0 = IMG::LookupPageFormat.new();
      lut1 = IMG::LookupPageFormat.new();
      lut2 = IMG::LookupPageFormat.new();
      lut3 = IMG::LookupPageFormat.new();
      screen_count = 0;
      progressbar=ProgressBar.create(:title => "Screen", :starting_at => 0, 
                                     :total => num_screens,
                                     :format => '%t %c/%C |%B| %e')
      for file in Dir.entries(basedir)
        if file =~ /^img-(\d+)-(\d+).png/ 
          temp, hum = $1.to_i, $2.to_i
          puts "File #{file}: Temp #{temp}, Hum #{hum}" if $verbose
          image = nil;
          begin
            image = ChunkyPNG::Image.from_file(File.join(basedir, file));
          rescue
            puts "Failed to read PNG image data from file " \
              << file << ". Exiting."
              exit;
          end
          if (image.dimension.height == EPD27::LINES_PER_DISPLAY &&
              image.dimension.width == EPD27::PIXEL_PER_LINE)
            screen = img2screen(image);
            flash.screens[screen_count] = screen;
            lut_line = IMG::LookupTableFormat.new();
            lut_line.temperature = temp;
            lut_line.humidity = hum;
            # The first page of the flash is the lookup table - so add
            # $LUT_PAGES to the offset.
            lut_line.startpage = screen_count * EPD27::PAGES_PER_SCREEN + IMG::LUT_PAGES;
            case screen_count
            when 0..IMG::LUTLINES_PER_PAGE-1
              lut0.lines[screen_count] = lut_line;
            when IMG::LUTLINES_PER_PAGE..(2*IMG::LUTLINES_PER_PAGE-1)
              lut1.lines[screen_count-IMG::LUTLINES_PER_PAGE] = lut_line;
            when (2*IMG::LUTLINES_PER_PAGE)..(3*IMG::LUTLINES_PER_PAGE-1)
              lut2.lines[screen_count-2*IMG::LUTLINES_PER_PAGE] = lut_line;
            when (3*IMG::LUTLINES_PER_PAGE)..(4*IMG::LUTLINES_PER_PAGE-1)
              lut3.lines[screen_count-3*IMG::LUTLINES_PER_PAGE] = lut_line;
            else
              puts "Cannot assign lookup table line to AT45 memory page - exiting."
              exit
            end
            screen_count += 1;
            progressbar.increment
          else
            puts "Failure: file " << file << " has wrong dimensions.";
            exit
          end

        else 
          puts "Ignoring #{file}" if $verbose
        end
      end

      # Don't forget to add the lookup table to the image
      flash.lut_pages[0]=lut0;
      flash.lut_pages[1]=lut1;
      flash.lut_pages[2]=lut2;
      flash.lut_pages[3]=lut3;
      return flash;
    end

  end
end
