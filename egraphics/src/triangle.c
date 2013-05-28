/**
 * \file triangle.h
 */
#include <esic/egraphics/triangle.h>

static const vtable_Object s_object_vtable = {
	Triangle_destructor,
	Triangle_clone,
	Triangle_equalsTo,
	Triangle_hash
};

static const vtable_Shape s_shape_vtable = {
	Triangle_paint
};

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

void Triangle_paint(PShape self, PAbstractPainter painter) {

}

void _sort_vertices_ascending_by_y(PVertice v1, PVertice v2, PVertice v3) {
	if(v1->y > v2->y) {
		VERTICE_SWAP(v1, v2);
	}

	/* here v1.y <= v2.y */
	if(v1->y > v3->y) {
		VERTICE_SWAP(v1, v2);
	}

	/* here v1.y <= v2.y and v1.y <= v3.y so test v2 vs. v3 */
	if(v2->y > v3->y) {
		VERTICE_SWAP(v2, v3);
	}
}

SDWORD _cross_product(DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	return x1 * y2 - y1 * x2;
}