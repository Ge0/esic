#include <esic/egraphics/geometrical_renderer.h>
#include <esic/egraphics/triangle.h>
#include <string.h>

/* Test: direct access to the lcd first */
#include <esic/elcd/lcd.h>

VTABLE_START(Object) {
	GeometricalRenderer_destructor,
	GeometricalRenderer_clone,
	GeometricalRenderer_equalsTo,
	GeometricalRenderer_hash,
	GeometricalRenderer_type
};

VTABLE_START(Renderer) {
	GeometricalRenderer_drawLine,
	GeometricalRenderer_drawRectangle,
	GeometricalRenderer_drawTriangle,
	GeometricalRenderer_drawBuffer,
	GeometricalRenderer_drawPixel,
	GeometricalRenderer_drawString
};

void _triangle_sort_vertices_ascending_by_y(PVertice v1, PVertice v2, PVertice v3);
void _triangle_fill_flat_side_triangle_int(PGeometricalRenderer self, const PVertice v1, const PVertice v2, const PVertice v3, DWORD color);

PGeometricalRenderer GeometricalRenderer_constructor(PGeometricalRenderer self) {
	OBJECT(self)->size     = sizeof(GeometricalRenderer);
	OBJECT(self)->vtable   = VTABLE_POINTER(Object);
	RENDERER(self)->vtable = VTABLE_POINTER(Renderer);
	return self;
}

void GeometricalRenderer_destructor(PObject self) {
}

PObject GeometricalRenderer_clone(PObject self, PObject dst) {
	dst->size = self->size;
	dst->vtable = self->vtable;

	RENDERER(dst)->vtable = RENDERER(self)->vtable;

	return dst;
}

BOOL GeometricalRenderer_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD GeometricalRenderer_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD GeometricalRenderer_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("GeometricalRenderer");
	}
	return s_hash;
}

void GeometricalRenderer_drawLine(PRenderer self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	DWORD deltax;
	DWORD deltay;
	double error;
	DWORD ystep;
	DWORD x;
	DWORD y;
	BYTE steep;
	
	steep = abs((long)y2 - (long)y1) > abs((long)x2 - (long)x1);

	if(steep) {
		SWAP(x1, y1);
		SWAP(x2, y2);
	}

	if(x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	deltax = x2 - x1;
	deltay = abs((long)y2 - (long)y1);
	error = deltax / 2;
	y = y1;

	if(y1 < y2) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for(x = x1; x <= x2; ++x) {
		if(steep) {
			//LcdPainter_drawPixel(self, y, x, color);
			LcdSetPixel(y, x, color);
		} else {
			//LcdPainter_drawPixel(self, x, y, color);
			LcdSetPixel(x, y, color);
		}


		error -= deltay;

		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}
}

void GeometricalRenderer_drawRectangle(PRenderer self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD background_color, DWORD border_color) {
	/* Firstly: draw the rectangle */
	DWORD i;
	for(i = 0; i < height; i++) {
		RENDERER_VTABLE(self)->drawLine(self, x, y + i, x + width-1, y + i, background_color);
	}

		/* Secondly, draw the border */

	RENDERER_VTABLE(self)->drawLine(self, x, y, x + width-1, y, border_color);						/* top one */
	RENDERER_VTABLE(self)->drawLine(self, x + width-1, y, x + width-1, y + (height-1), border_color);	/* right one */
	RENDERER_VTABLE(self)->drawLine(self, x + width-1, y + (height-1), x, y + (height-1), border_color);	/* bottom one */
	RENDERER_VTABLE(self)->drawLine(self, x, y + (height-1), x, y, border_color);					/* left one */
}

void GeometricalRenderer_drawTriangle(PRenderer self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color) {
	Vertice v1, v2, v3;

	Vertice_constructor(&v1);
	Vertice_constructor(&v2);
	Vertice_constructor(&v3);

	v1.x = x0;
	v1.y = y0;
	v2.x = x1;
	v2.y = y1;
	v3.x = x2;
	v3.y = y2;

	//Triangle_paint(SHAPE(&triangle), self);

	_triangle_sort_vertices_ascending_by_y(&v1, &v2, &v3);

	if(v2.y == v3.y) {
		_triangle_fill_flat_side_triangle_int(GEOMETRICALRENDERER(self), &v1, &v2, &v3, filling_color);
	} else if(v1.y == v2.y) {
		_triangle_fill_flat_side_triangle_int(GEOMETRICALRENDERER(self), &v3, &v1, &v2, filling_color);
	} else {
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vertice vtmp;
		Vertice_constructor(&vtmp);

		vtmp.x = (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x));
		vtmp.y = v2.y;

		_triangle_fill_flat_side_triangle_int(GEOMETRICALRENDERER(self), &v1, &v2, &vtmp, filling_color);
		_triangle_fill_flat_side_triangle_int(GEOMETRICALRENDERER(self), &v3, &v2, &vtmp, filling_color);
	}

	/* Draw borders */
	self->vtable->drawLine(
		self,
		v1.x,
		v1.y,
		v2.x,
		v2.y,
		border_color
	);

	self->vtable->drawLine(
		self,
		v2.x,
		v2.y,
		v3.x,
		v3.y,
		border_color
	);

	self->vtable->drawLine(
		self,
		v3.x,
		v3.y,
		v1.x,
		v1.y,
		border_color
	);
}

void GeometricalRenderer_drawBuffer(PRenderer self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer) {
	DWORD i, j;
	DWORD pixel = 0;
	BOOL condition;
	WORD* word_buffer = (WORD*)raw_buffer;
	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			switch(bpp) {
			case 1:
				pixel = (BYTE)*((BYTE*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xff);
				break;
			case 2:
				pixel = (WORD)*((WORD*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xffff);
				//LcdSetPixel(x+i, y+j, word_buffer[j * width + i]);
				break;

			case 4:
				pixel = *((DWORD*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xffffffff);
				break;

			default:
				condition = FALSE;
				pixel = 0;
			}
			
			//if(raw_buffer[j * width + i] != 0xffff) {
			
			if(condition) {
				LcdSetPixel(x+i, y+j, pixel);
			}
		}
	}
}

void GeometricalRenderer_drawString(PRenderer self, PRasterFont font, DWORD x, DWORD y, DWORD color, const char* string) {
	WORD i, j;
	DWORD len;
	BYTE character_width, character_height;
	DWORD size_string = strlen(string);
	char* character_data = NULL;


	/* For each character */
	for(i = 0; i < size_string; ++i) {
		int utf8_code = 0;

		/* If the code of the char is above 0x7F, then this is UTF-8) */
		if(string[i] > 0x7F) {
			/* TODO: draw unicode character */
		} else {
			/* Sample ASCII */
			utf8_code = string[i];
		}

		/* Retrieve the character's data */
		character_data = RasterFont_getCharacterData(font, utf8_code, &len, &character_width, &character_height);

		/* Data found? Map it into the framebuffer ! */
		if(character_data != NULL) {

			for(j = 0; j < len*8; j++) {
				DWORD bit = GET_BIT_STRING_BIGENDIAN(character_data, j);
				/* If the current bit is 1, switch the pixel on */
				if(bit == 1) {
					WORD x_dest = (x + i * character_width + (j % character_width));
					WORD y_dest = y + (j / character_width) /* + ((x_dest / 320) * character_height) */;
					LcdSetPixel(x_dest, y_dest, color);
				}
			}

			SicFree(character_data);

		}
	}
}

void GeometricalRenderer_drawPixel(PRenderer self, DWORD x, DWORD y, DWORD color) {
	LcdSetPixel(x, y, color);
}

void _triangle_sort_vertices_ascending_by_y(PVertice v1, PVertice v2, PVertice v3) {
	if(v1->y > v2->y) {
		VERTICE_SWAP((*v1), (*v2));
	}

	/* here v1.y <= v2.y */
	if(v1->y > v3->y) {
		VERTICE_SWAP((*v1), (*v3));
	}

	/* here v1.y <= v2.y and v1.y <= v3.y so test v2 vs. v3 */
	if(v2->y > v3->y) {
		VERTICE_SWAP((*v2), (*v3));
	}
}

void _triangle_fill_flat_side_triangle_int(PGeometricalRenderer self, const PVertice v1, const PVertice v2, const PVertice v3, DWORD color) {
	Vertice tmp_vertice_1;
	Vertice tmp_vertice_2;

	BOOL changed1 = FALSE;
	BOOL changed2 = FALSE;

	SDWORD e1;
	SDWORD e2;

	int i;

	SDWORD dx1 = abs((long long)v2->x - v1->x);
	SDWORD dy1 = abs((long long)v2->y - v1->y);

	SDWORD dx2 = abs((long long)v3->x - v1->x);
	SDWORD dy2 = abs((long long)v3->y - v1->y);

	SWORD signx1 = Sign(v2->x - v1->x);
	SWORD signx2 = Sign(v3->x - v1->x);

	SWORD signy1 = Sign(v2->y - v1->y);
	SWORD signy2 = Sign(v3->y - v1->y);

	
	if(dy1 > dx1) {
		/* Swap values */
		SWAP(dx1, dy1);
		changed1 = TRUE;
	}

	if(dy2 > dx2) {
		/* Swap values */
		SWAP(dx2, dy2);
		changed2 = TRUE;
	}

	e1 = 2 * dy1 - dx1;
	e2 = 2 * dy2 - dx2;


	Vertice_constructor(&tmp_vertice_1);
	Vertice_constructor(&tmp_vertice_2);

	tmp_vertice_1.x = tmp_vertice_2.x = v1->x;
	tmp_vertice_1.y = tmp_vertice_2.y = v1->y;

	for(i = 0; i <= dx1; ++i) {

		RENDERER_VTABLE(self)->drawLine(
			RENDERER(self),
			tmp_vertice_1.x,
			tmp_vertice_1.y,
			tmp_vertice_2.x,
			tmp_vertice_2.y,
			color
		);

		while(e1 >= 0) {
			if(changed1) {
				tmp_vertice_1.x += signx1;
			} else {
				tmp_vertice_1.y += signy1;
			}

			e1 = e1 - 2 * dx1;
		}

		if(changed1) {
			tmp_vertice_1.y += signy1;
		} else {
			tmp_vertice_1.x += signx1;
		}

		e1 = e1 + 2 * dy1;

		/* here we rendered the next point on line 1 so follow now line 2
		* until we are on the same y-value as line 1.
		*/
		while(tmp_vertice_2.y != tmp_vertice_1.y) {
			while(e2 >= 0) {
				if(changed2) {
					tmp_vertice_2.x += signx2;
				} else {
					tmp_vertice_2.y += signy2;
				}

				e2 = e2 - 2 * dx2;
			}

			if(changed2) {
				tmp_vertice_2.y += signy2;
			} else {
				tmp_vertice_2.x += signx2;
			}

			e2 = e2 + 2 * dy2;
		}
	}

	Vertice_destructor(OBJECT(&tmp_vertice_1));
	Vertice_destructor(OBJECT(&tmp_vertice_2));
}
