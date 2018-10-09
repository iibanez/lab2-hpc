f = fopen('salida3.raw', 'r');
M = fread(f, 'float');
M = reshape(M, 2000, 2000);
imagesc(M);
colormap( [jet();flipud( jet() );0 0 0] );