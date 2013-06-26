/**
 * \file triangle.h
 */
#include <esic/egraphics/triangle.h>
#include <esic/eapi/misc.h>

static const vtable_Object s_object_vtable = {
	Triangle_destructor,
	Triangle_clone,
	Triangle_equalsTo,
	Triangle_hash
};

static const vtable_Shape s_shape_vtable = {
	Triangle_paint
};

void _fill_flat_side_triangle_int(PTriangle self, PPainter painter, const PVertice v1, const PVertice v2, const PVertice v3);
void _sort_vertices_ascending_by_y(PTriangle self);

PTriangle Triangle_constructor(PTriangle self) {
	self->shape.object.size = sizeof(Triangle);
	self->shape.object.vtable = &s_object_vtable;
	self->shape.vtable = &s_shape_vtable;

	Vertice_constructor(&self->v1);
	Vertice_constructor(&self->v2);
	Vertice_constructor(&self->v3);

	return self;
}

void Triangle_destructor(PObject self) {

}

PObject Triangle_clone(PObject self, PObject dst) {
	PTriangle real_self = (PTriangle) self;
	PTriangle real_dst  = (PTriangle) dst;

	dst->vtable = self->vtable;
	dst->size   = self->size;

	real_dst->v1 = real_self->v1;
	real_dst->v2 = real_self->v2;
	real_dst->v3 = real_self->v3;


	return dst;

}

BOOL Triangle_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Triangle_hash(PObject self) {
	/* TODO. */
	return 0;
}

void Triangle_paint(PShape self, PPainter painter) {
	_sort_vertices_ascending_by_y(TRIANGLE(self));

	if(TRIANGLE(self)->v2.y == TRIANGLE(self)->v3.y) {
		_fill_flat_side_triangle_int(TRIANGLE(self), painter, &TRIANGLE(self)->v1, &TRIANGLE(self)->v2, &TRIANGLE(self)->v3);
	} else if(TRIANGLE(self)->v1.y == TRIANGLE(self)->v2.y) {
		_fill_flat_side_triangle_int(TRIANGLE(self), painter, &TRIANGLE(self)->v3, &TRIANGLE(self)->v1, &TRIANGLE(self)->v2);
	} else {
		/* general case - split the triangle in a topflat and bottom-flat one */
		Vertice vtmp;
		Vertice_constructor(&vtmp);

		vtmp.x = (int)(TRIANGLE(self)->v1.x + ((float)(TRIANGLE(self)->v2.y - TRIANGLE(self)->v1.y) / (float)(TRIANGLE(self)->v3.y - TRIANGLE(self)->v1.y)) * (TRIANGLE(self)->v3.x - TRIANGLE(self)->v1.x));
		vtmp.y = TRIANGLE(self)->v2.y;

		_fill_flat_side_triangle_int(TRIANGLE(self), painter, &TRIANGLE(self)->v1, &TRIANGLE(self)->v2, &vtmp);
		_fill_flat_side_triangle_int(TRIANGLE(self), painter, &TRIANGLE(self)->v3, &TRIANGLE(self)->v2, &vtmp);
	}

	/* Draw borders */
	/*
	painter->drawLine(
		painter,
		TRIANGLE(self)->v1.x,
		TRIANGLE(self)->v1.y,
		TRIANGLE(self)->v2.x,
		TRIANGLE(self)->v2.y,
		self->border_color
	);

	ABSTRACTPAINTER_VTABLE(painter)->drawLine(
		painter,
		TRIANGLE(self)->v2.x,
		TRIANGLE(self)->v2.y,
		TRIANGLE(self)->v3.x,
		TRIANGLE(self)->v3.y,
		self->border_color
	);

	ABSTRACTPAINTER_VTABLE(painter)->drawLine(
		painter,
		TRIANGLE(self)->v3.x,
		TRIANGLE(self)->v3.y,
		TRIANGLE(self)->v1.x,
		TRIANGLE(self)->v1.y,
		self->border_color
	);
	*/
}

void _sort_vertices_ascending_by_y(PTriangle self) {
	if(self->v1.y > self->v2.y) {
		VERTICE_SWAP(self->v1, self->v2);
	}

	/* here v1.y <= v2.y */
	if(self->v1.y > self->v3.y) {
		VERTICE_SWAP(self->v1, self->v3);
	}

	/* here v1.y <= v2.y and v1.y <= v3.y so test v2 vs. v3 */
	if(self->v2.y > self->v3.y) {
		VERTICE_SWAP(self->v2, self->v3);
	}
}

SDWORD _cross_product(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return x1 * y2 - y1 * x2;
}

void _fill_flat_side_triangle_int(PTriangle self, PPainter abstract_painter, const PVertice v1, const PVertice v2, const PVertice v3) {
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
		/*
		ABSTRACTPAINTER_VTABLE(abstract_painter)->drawLine(
			abstract_painter,
			tmp_vertice_1.x,
			tmp_vertice_1.y,
			tmp_vertice_2.x,
			tmp_vertice_2.y,
			self->shape.color
		);
		*/
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