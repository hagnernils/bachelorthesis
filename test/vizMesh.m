%data = dlmread("meshData.txt");
%a = readmatrix("mesh.out", 'FileType', '');

verts = readmatrix("meshVerts.txt");
indices = readmatrix("meshIndices.txt");

data = readmatrix("meshData.txt");

numtris = 3000 -1;

x = verts (1:end, 1);
y = verts (1:end, 2);
z = verts (1:end, 3);
c = verts (1:end, 4);

connectivity = indices(1:end, 1:3);

trisurf(connectivity, x, y, z, c)
h=colorbar;
t=get(h,'Limits');
set(h,'Ticks', linspace ( t (1), t (2), 5));
axis off;
axis equal;
%tris = reshape (0:35, 12, 3);
%tris = tris.';
%title (sprintf ("facecolor = %s", get (trisurf (tris, x, y, z, "facecolor", "interp"), "facecolor")));