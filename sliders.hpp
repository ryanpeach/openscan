#ifndef SLIDER
#define SLIDER

struct udata {
	Capture *const c;
	const Par p;
	udata(Capture *c, Par param) : c(c), p(param) {};
};

void setTrackBar(int newv, void* ptr) {
	udata *u = (udata*)ptr;
	(u->c)->setValue(u->p,newv);
}

struct Slider {
	int v;
    Capture *const c;
    const Par param;
    const String vname;
    const String wname;

    const udata u = udata(c,param);

    Slider(String vname, String wname, Par p, Capture* c, int maxv) :
    	c(c), param(p), vname(vname), wname(wname) {
    	v = c->getValue(param);
    	createTrackbar(vname, wname, &v, maxv, &setTrackBar, (void*)&u);
    }
};

#endif
