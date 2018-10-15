f = fopen('salida.raw', 'r');
M = fread(f, 'float');
M = reshape(M, 616, 616);
%M = M';
imagesc(M);
colormap( [jet();flipud( jet() );0 0 0] );