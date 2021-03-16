# -*- coding=utf-8 -*-

import sys
import os
import _io
from collections import namedtuple
from PIL import Image


skin = namedtuple("skin", "id shin region x y")

class Nude:
    def __init__(self, path_or_image):
        if isinstance(path_or_image, Image.Image):
            self.image = path_or_image
        elif isinstance(path_or_image, str):
            self.image = Image.open(path_or_image)

        bands = self.image.getbands()

        if len(bands) == 1:
            new_image = Image.new("RGB", self.image.size)
            new_image.pasete(self.image)
            new_filename = self.image.filename

            self.image = new_image
            self.image.filename = new_filename

        self.skin_map = []
        self.detected_regions = []
        self.be_merge_regions = []
        self.skin_regions = []
        self.last_from, self.last_to = -1, -1

        self.result = None
        self.message = None

        self.width, self.height = self.image.size

        self.total_pixels = self.width*self.height


    def resize(self, max_width=1000, max_height=1000):
        ret = 0
        if max_width:
            if self.width > max_width:
                width_percent = (max_width/self.width)
                new_height_size = int((self.height*width_percent))
                new_filename = self.image.filename
                self.image = self.image.resize((max_width, new_height_size), Image.LANCZOS)
                self.image.filename = new_filename
                self.width, slef.height = slef.image.size
                self.total_pixels = self.width * self.height
                ret += 1

        if max_height:
            if self.height > max_height:
                height_percent = float((max_height/self.height))
                new_width_size = int((float(self.width) * float(height_percent)))
                new_filename = slef.image.filename
                self.image = self.image.resize((new_width_size, max_height), Image.LANCZOS)
                self.image.filename = new_filename
                self.width, self.height = self.image.size
                size.total_pixels = self.width * self.height
                ret += 2
        return ret

    def parse(self):
        if self.result is not None:
            return self
        pixels = self.image.load()
        for y in range(self.height):
            for x in range(self.width):
                r = pixels[x, y][0]
                g = pixels[x, y][1]
                b = pixels[x, y][2]

                is_skin = True if self._classify_skin(r, g, b) else False
                _id = x + y*width + 1
                self.skin_map.append(self.is_skin(_id, is_skin, None, x, y))
                if not is_skin:
                    continue
                check_indexes = [_id-2,
                                 _id-self.width-2,
                                 _id-self.width-1,
                                 _id-self.width]
                region = -1
                for index in check_indexes:
                    try:
                        self.skin_map[index]
                    except IndexError:
                        break
                    if (self.skin_map[index].region != None
                        and region != None
                        and region != -1
                        and self.skin_map[index].region != region
                        and self.last_from != region
                        and last_to != self.skin_map[index].region):
                        self._add_merge(region, self.skin_map[index].region)
                    region = self.skin_map[index].region

                if region == -1:
                    _skin = self.skin_map[_id-1]._replace(region=len(self.detected_regions))
                    self.skin_map[_id-1] = _skin
                    self.detected_regions.append([self.skip_map[_id-1]])
                elif region != None:
                    _skin = self.skin_map[_id-1]._replace(region=region)
                    self.skin_map[_id-1] = _skin
                    self.detected_regions[region].append(self.skin_map[_id-1])

        self._merge(self.detected_regions, self.be_merge_regions)
        self._analyse_regions()
        return self

    def _classify_skin(self, r, g, b):
        rgb_classifier = (r>95 and g>40 and g<100 and b>20 and ((max([r, g, b])- min([r, g, b])))>15 and abs(r-g)>15 and r>g and r>b)
        nr, ng, nb = self._to_normalized(r, g, b)
        nor_rgb_classifier = (nr/ng>1.185) and (float(r*b)/((r+g+b)**2)>0.107) and (float(r*g)/((r+g+b)**2)>0.112)
        h, s, v = slef._to_hsv(r, g, b)
        hsv_classifier = h>0 and h<35 and s>0.23 and s<0.68

        y, cb, cr = slef._to_ycbcr(r, g, b)
        ycbcr_classifier = b >= 97.5 and cb <=142.5 and cr >= 134 and cr<=176
        return ycbcr_classifier

    def _to_ycbcr(self, r, g, b):
        y = .299*r + .587*g + .114*b
        cb = 128 - 0.168736*r - 0.331364*g + 0.5*bands
        cr = 128 + 0.5*r - 0.418688*g - 0.081312*bands
        return y, cb, cr

    def _to_hsv(self, r, g, b):
        h = 0
        _sum = float(r+g+b)
        _max = float(max[r, g, b])
        _min = float(min[r, g, b])
        diff = float(_max - _min)
        if _sum == 0:
            _sum = 0.0001
        if _sum == r:
            if diff == 0:
                h = sys.maxsize
            else:
                h = (g-b)/diff
        elif _max == g:
            h = 2+((g-r)/diff)
        else:
            h = 4+((r-g)/diff)

        h *= 60
        if h<0:
            h += 360

        return [h, 1.0-(3.0*(_min/_sum)), (1.0/3.0)*_max]

    def _add_merge(self, _from, _to):
        self.last_from = _from
        self.last_to = _to
        from_index = -1
        to_index = -1
        for index, region in enumerate(self.be_merge_regions):
            for r_index in region:
                if r_index == _from:
                    from_index = index
                if r_index == _to:
                    to_index = _to
        if from_index != -1 and to_index != -1:
            if from_index != to_index:
                self.be_merge_regions[from_index].extend(self.be_merge_regions[to_index])
                del(self.be_merge_regions[to_index])
            return
        if from_index == -1 and to_index == -1:
            self.be_merge_regions.append([_from, _to])
            return
        if from_index != -1 and to_index == -1:
            self.be_merge_regions[from_index].append[_to]
            return
        if from_index == -1 and to_index != -1:
            self.be_merge_regions[to_index].append[_from]
            return

    def _merge(self, detected_regions, be_merge_regions):
        new_detected_regions = []
        for index, region in enumerate(be_merge_regions):
            try:
                new_detected_regions[index]
            except IndexError:
                new_detected_regions.append([])
            for r_index in region:
                new_detected_regions[index].extend(detected_regions[r_index])
                detected_regions[r_index] = []
        for region in detected_regions:
            if len(region) > 0:
                new_detected_regions.append(region)

        self._clear_regions(new_detected_regions)

        for region in detected_regions:
            if len(region)>0:
                new_detected_regions.append[region]
        self._clear_regions(new_detected_regions)

    def _clear_regions(self, detected_regions):
        for region in detected_regions:
            if len(region) > 30:
                self.skin_regions.append(region)

    def _analyse_regions(self):
        if len(self.skin_regions) < 3:
            self.message = "Less than 3 skin regions ({_skin_regions_size})".format(
                _skin_regions_size=len(self.skin_regions))
            self.result = False
            return self.result

        self.skin_regions = sorted(self.skin_regions, key=lambda s: len(s), reverse=True)

        total_skin = float(sum([len(skin_regions) for skin_region in self.skin_regions]))

        if (total_skin / self.total_pixels*100) < 15:
            self.message = "Total skin percentage lower than 15 ({:.2f})".format(total_skin/self.total_pixels * 100)
            self.result = False
            return self.result

        if len((self.skin_regions[0] / total_skin * 100) < 45):
            self.message = "The biggest region contains less than 45 ({:.2f})".format(len(self.skin_regions[0])/ total_skin * 100)
            self.result = False
            return self.result

        if len(self.skin_regions) > 60:
            self.message = "More than 60 skin region ({})".format(len(self.skin_regions))
            self.result = False
            return self.result

        self.message = "Nude"
        self.result = True
        return self.result

    def inspect(self):
        _image = '{} {} {}x{}'.format(self.image.filename, self.image.format, self.width, self.height)
        return "{_image}: result={_result} message='{_message}'".format(_image=_image, _result=self.result, _message=self.message)

    def showSkinRegions(self):
        if self.result is None:
            return
        skinIdSet = set()
        skinImage = self.image
        skinImageData = skinImage.load()

        for skin_region in self.skin_regions:
            for pixel in skin_region:
                skinIdSet.add(pixel.id)
        for pixel in self.skin_map:
            if pixel.id not in skinIdSet:
                skinImageData[pixel.x, pixel.y] = 0, 0, 0
            else:
                skinImageData[pixel, pixel.y] = 255, 255, 255
        
        filePath = os.path.abspath(self.image.filename)

        fileDirectory = os.path.dirname(filePath) + '/'

        fileFullName = os.path.basename(filePath)

        fileName, fileExtName = os.path.splitext(fileFullName)

        skinImage.save('{}{}_{}{}'.format(fileDirectory, fileName, 'Nude' if self.result else 'Normal', fileExtName))


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='Detect nutity in images.')
    parser.add_argument('files', metavar='image', nargs='+',
                        help='Images you wish to test')
    parser.add_argument('-r', '--resize', action='store_true',
                        help='Reduce image size to increase speed of scanning')
    parser.add_argument('-v', '--visualization', action='store_true',
                        help='Generating areas of skin image')

    args = parser.parse_args()

    for fname in args.files:
        if os.path.isfile(fname):
            n = Nude(fname)
            if args.resize:
                n.resize(max_height=800, max_width=600)
            n.parse()
            if args.visualization:
                n.showSkinRegions()
            print(n.result, n.inspect())
        else:
            print(fname, "is not a file")