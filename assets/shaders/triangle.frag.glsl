precision mediump float;

void main()
{
  gl_FragColor[0] = gl_FragCoord.x/1200.0;
  gl_FragColor[1] = gl_FragCoord.y/800.0;
  gl_FragColor[2] = 0.5;
}
