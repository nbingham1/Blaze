/*
 * img.h
 * Blaze Game Engine v0.11
 *
 * Created by Ned Bingham on June 16, 2012.
 * Modified by Ned Bingham on June 16, 2012.
 * Copyright 2011 Sol Union. All rights reserved.
 *
 * Blaze Game Engine v0.11 is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Blaze Game Engine v0.11 is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Blaze Game Engine v0.11.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This code was derived from http://eeg.sourceforge.net/ANALYZE75.pdf
 */

#include "../../common.h"

#ifndef texture_img_h
#define texture_img_h

// Acceptable values for datatype
#define IMG_NONE			0x00
#define IMG_UNKNOWN			0x00
#define IMG_BINARY			0x01
#define IMG_UNSIGNED_CHAR	0x02
#define IMG_SIGNED_SHORT	0x04
#define IMG_SIGNED_INT		0x08
#define IMG_FLOAT			0x10
#define IMG_COMPLEX			0x20
#define IMG_DOUBLE			0x40
#define IMG_RGB				0x80
#define IMG_ALL				0xff

// Acceptable values for orient
#define IMG_TRANSVERSE_UNFLIPPED	0	// View is from underneath, first section is the bottom.
#define IMG_CORONAL_UNFLIPPED		1	// View is from front, first section is the back.
#define IMG_SAGITTAL_UNFLIPPED		2	// View is from right side, first section is the right side.
#define IMG_TRANSVERSE_FLIPPED		3	// View is from underneath, first section is the top.
#define IMG_CORONAL_FLIPPED			4	// View is from front, first section is the front.
#define IMG_SAGITTAL_FLIPPED		5	// View is from right side, first section is the left side.

struct img_header_key
{
	uint32_t sizeof_hdr;	// Must indicate the byte size of the header file
	uint8_t  data_type[10];
	uint8_t  db_name[18];
	uint32_t extents;		// Should be 16384, the image file is created as contiguous with a minimum extent size
	uint16_t session_error;
	uint8_t  regular;		// Must be `r' to indicate that all images and volumes are the same size
	uint8_t  unused0;
};

struct img_dimension
{
	/* dim[]  Array of the image dimensions
	 * dim[0] Number of dimensions in database; usually 4
	 * dim[1] Image X dimension; number of pixels in an image row
	 * dim[2] Image Y dimension; number of pixel rows in slice
	 * dim[3] Volume Z dimension; number of slices in a volume
	 * dim[4] Time points, number of volumes in database.
	 *
	 */
	uint16_t dim[8];
	uint16_t unused0[7];
	uint16_t datatype;		// Datatype for this image set (see above)

	/* Number of bits per pixel: 1, 8, 16, 32, or 64
	 * 1 bit packed binary (slices must begin on byte boundaries)
	 * 8 bit 8 bits per pixel (unsigned char)
	 * 16 bit 16 bits per pixel (signed short)
	 * 24 bit RGB, 8-bits per channel Red, Green, Blue.
	 * 32 bit 32 bits per pixel signed integers, or floating point
	 * 64 bit 64 bits per pixel; double precision, floating point, or complex.
	 */
	uint16_t bitpix;
	uint16_t unused1;

	/* pixdim[]  Parallel array to dim[], giving real world measurements in mm. and ms.
	 * pixdim[1] Voxel width in mm
	 * pixdim[2] Voxel height in mm
	 * pixdim[3] Interslice distance in mm
	 * ...
	 */
	float    pixdim[8];

	/* Byte offset in the .img file at which voxels start.
	 * This value can negative to specify that the absolute
	 * value is applied for every image in the file.
	 */
	int32_t  vox_offset;
	float    unused2[3];
	float    cal_max;		// Specify the range of calibration values
	float    cal_min;
	float    compressed;
	float    verified;
	uint32_t glmax;			// The maximum and minimum pixel values for the entire database
	uint32_t glmin;
};

struct img_data_history
{
	uint8_t  description[80];
	uint8_t  aux_file[24];
	uint8_t  orient;			// slice orientation for this dataset (see above)
	uint8_t  originator[10];
	uint8_t  generated[10];
	uint8_t  scannum[10];
	uint8_t  patient_id[10];
	uint8_t  exp_date[10];
	uint8_t  exp_time[10];
	uint8_t  hist_un0[3];
	uint32_t views;
	uint32_t vols_added;
	uint32_t start_field;
	uint32_t field_skip;
	uint32_t omax;
	uint32_t omin;
	uint32_t smax;
	uint32_t smin;
};

struct img_header
{
	img_header_key		key;
	img_dimension		dim;
	img_data_history	hist;
};

#endif
