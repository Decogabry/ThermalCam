#pragma once
#include <pgmspace.h>

const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html><html lang=en><head><meta charset=utf-8>
<meta name=viewport content="width=device-width,initial-scale=1,viewport-fit=cover,user-scalable=no">
<meta name=apple-mobile-web-app-capable content=yes>
<meta name=apple-mobile-web-app-status-bar-style content=black>
<meta name=theme-color content=#101014>
<title>Thermal Camera</title><style>
:root{--bg:#101014;--s1:#1B1B1F;--s2:#26262B;--s3:#33333A;--ol:#45464F;
--on:#E5E2E9;--on2:#A9A8B3;--ac:#A8C7FA;--rc:#F2555A}
*{box-sizing:border-box;-webkit-tap-highlight-color:transparent;margin:0}
html,body{height:100%;overflow:hidden}
body{background:var(--bg);color:var(--on);display:flex;flex-direction:column;
 font:15px/1.4 -apple-system,system-ui,Roboto,sans-serif;
 padding-top:env(safe-area-inset-top);padding-bottom:env(safe-area-inset-bottom)}
.n{font-family:ui-monospace,Menlo,monospace;font-variant-numeric:tabular-nums}
.view{flex:none}
.stage{position:relative;width:100%;aspect-ratio:4/3;background:#000}
canvas#v{display:block;width:100%;height:100%;touch-action:none}
.hud{position:absolute;top:0;right:0;display:flex;justify-content:flex-end;
 padding:10px 14px;font-size:12px;color:#fff;text-shadow:0 1px 3px #000;pointer-events:none}
.hud b{font-weight:400;display:flex;align-items:center;gap:6px}
.d{width:8px;height:8px;border-radius:50%;background:#77767F}
.d.ok{background:#77E39A}.d.no{background:var(--rc)}
.rec{position:absolute;top:34px;left:50%;transform:translateX(-50%);
 display:none;align-items:center;gap:7px;background:rgba(0,0,0,.6);
 padding:5px 12px;border-radius:20px;font-size:13px}
.rec.on{display:flex}
.rec i{width:9px;height:9px;border-radius:50%;background:var(--rc);
 animation:bl 1s steps(2,end) infinite}
@keyframes bl{50%{opacity:.2}}
.panel{flex:1;display:flex;flex-direction:column;justify-content:space-between;
 padding:12px 16px 6px;min-height:0;overflow:hidden}
.read{display:flex;align-items:flex-end;justify-content:space-between}
.big{font-size:50px;line-height:.9;font-weight:200;letter-spacing:-.03em}
.big u{text-decoration:none;font-size:17px;color:var(--on2);margin-left:2px}
.cap{font-size:11px;color:var(--on2);margin-top:5px}
.ends{text-align:right;font-size:15px}
.ends div{display:flex;gap:8px;justify-content:flex-end;line-height:1.5}
.ends s{text-decoration:none;font-size:11px;color:var(--on2);width:44px;text-align:left}
#hiv{color:#FF8A5C}#lov{color:#7FB8FF}
.scl{display:flex;align-items:center;gap:9px;font-size:12px;color:var(--on2);margin-top:11px}
#rp{flex:1;height:10px;display:block;border-radius:5px}
.dist{display:flex;align-items:flex-end;justify-content:space-between;
 margin-top:13px;padding-top:12px;border-top:1px solid var(--s3)}
.dist .big{font-size:34px}
.dist .big u{font-size:14px}
.dgeo{text-align:right;font-size:11px;color:var(--on2);line-height:1.6}
.dgeo b{font-weight:400;color:var(--on);font-family:ui-monospace,Menlo,monospace}
.off{opacity:.35}
.chips{display:flex;gap:8px;margin-top:12px}
.chips button{flex:1;border:1px solid var(--ol);background:transparent;color:var(--on2);
 font:13px inherit;padding:8px 6px;border-radius:8px;min-height:38px}
.chips button.on{background:var(--ac);border-color:var(--ac);color:#0A2744}
.acts{display:flex;align-items:center;justify-content:space-between;padding:8px 2px 4px}
.ib{width:46px;height:46px;border-radius:23px;border:0;background:var(--s2);
 color:var(--on);display:flex;align-items:center;justify-content:center;flex:none}
.ib.on{background:var(--ac);color:#0A2744}
.shut{width:68px;height:68px;border-radius:34px;border:3px solid var(--on);
 background:var(--on);padding:0;box-shadow:inset 0 0 0 3px var(--bg);flex:none}
.shut:active{transform:scale(.94)}
.vid{width:56px;height:56px;border-radius:28px;border:3px solid var(--s3);
 background:transparent;display:flex;align-items:center;justify-content:center;padding:0;flex:none}
.vid span{width:24px;height:24px;border-radius:12px;background:var(--rc);transition:.15s}
.vid.on span{width:18px;height:18px;border-radius:4px}
.scrim{position:fixed;inset:0;background:rgba(0,0,0,.5);z-index:8;display:none}
.scrim.on{display:block}
.sheet{position:fixed;left:0;right:0;bottom:0;background:var(--s1);
 border-radius:24px 24px 0 0;padding:10px 18px 30px;transform:translateY(105%);
 transition:transform .22s ease,opacity .18s ease;z-index:9;max-width:540px;margin:0 auto}
.sheet.on{transform:none}
.grab{width:34px;height:4px;border-radius:2px;background:var(--ol);margin:2px auto 16px}
.f{display:flex;align-items:center;gap:12px;padding:7px 0;font-size:14px}
.f label{flex:0 0 96px;color:var(--on2)}
.f input{flex:1;accent-color:var(--ac);min-width:0}
.f output{flex:0 0 46px;text-align:right}
.sw{display:flex;align-items:center;justify-content:space-between;padding:10px 0;font-size:14px}
.sw input{width:22px;height:22px;accent-color:var(--ac)}
.close{width:100%;margin-top:14px;border:0;border-radius:10px;background:var(--s2);
 color:var(--on);font:15px inherit;padding:14px}
.note{font-size:12px;color:var(--on2);padding-top:10px;line-height:1.5}
.keys{display:none;font-size:11px;color:var(--on2);padding:10px 2px 2px;
 border-top:1px solid var(--s3);margin-top:10px;line-height:1.7}
.keys kbd{font:inherit;font-family:ui-monospace,Menlo,monospace;background:var(--s2);
 border:1px solid var(--ol);border-radius:4px;padding:1px 5px;color:var(--on)}

@media (min-width:820px){
 body{flex-direction:row;padding:0}
 .view{flex:1;min-width:0;display:flex;align-items:center;justify-content:center;
  background:#08080A;padding:18px}
 .stage{max-width:calc((100vh - 36px) * 4 / 3);border-radius:5px;overflow:hidden}
 .panel{flex:0 0 380px;border-left:1px solid var(--s3);padding:22px 24px 14px;
  overflow-y:auto}
 .acts{padding:14px 2px 6px}
 .big{font-size:58px}
 .dist .big{font-size:38px}
 .keys{display:block}
 button{cursor:pointer}
 .chips button:hover{border-color:var(--on2);color:var(--on)}
 .ib:hover{background:var(--s3)}
 .scrim{background:rgba(0,0,0,.6)}
 .sheet{left:50%;right:auto;bottom:auto;top:50%;width:430px;
  border-radius:16px;padding:22px 24px;opacity:0;pointer-events:none;
  transform:translate(-50%,-48%)}
 .sheet.on{transform:translate(-50%,-50%);opacity:1;pointer-events:auto}
 .grab{display:none}
}
</style></head><body>

<div class=view>
 <div class=stage>
  <canvas id=v></canvas>
  <div class=hud><b><span class=d id=led></span><span id=msg>connecting</span>
   <span class=n id=fps></span></b></div>
  <div class=rec id=recbox><i></i><span class=n id=rect>0:00</span></div>
 </div>
</div>

<div class=panel>
 <div>
  <div class=read>
   <div>
    <div id=sp class="big n">--.-<u>&deg;C</u></div>
    <div class=cap>center spot</div>
   </div>
   <div class=ends><div><s>max</s><b id=hiv class=n>--.-</b></div><div><s>min</s><b id=lov class=n>--.-</b></div></div>
  </div>
  <div class=scl><span id=l0 class=n>--</span><canvas id=rp width=256 height=10></canvas><span id=l1 class=n>--</span></div>
  <div class=dist id=distrow>
   <div>
    <div id=dv class="big n">--.--<u>m</u></div>
    <div class=cap>target distance</div>
   </div>
   <div class=dgeo id=dgeo><div>1 px &asymp; <b>--</b> cm</div><div>field <b>--</b> cm</div></div>
  </div>
  <div class=chips id=gp>
   <button data-p=0 class=on>Ironbow</button><button data-p=1>Rainbow</button>
  </div>
  <div class=keys><kbd>space</kbd> freeze &nbsp; <kbd>R</kbd> rotate &nbsp;
   <kbd>P</kbd> photo &nbsp; <kbd>V</kbd> video &nbsp; <kbd>S</kbd> settings</div>
 </div>
 <div class=acts>
  <button class=ib id=bcfg aria-label=settings>
   <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor"
    stroke-width="1.8" stroke-linecap="round">
   <path d="M4 7h16M4 12h16M4 17h16"></path></svg></button>
  <button class=ib id=brot aria-label=rotate>
   <svg width="21" height="21" viewBox="0 0 24 24" fill="none" stroke="currentColor"
    stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round">
   <path d="M4.6 9.2a8 8 0 1 1 .2 5.8"></path><path d="M2.2 5v4.4h4.4"></path></svg></button>
  <button class=shut id=bshot aria-label=photo></button>
  <button class=vid id=bvid aria-label=video><span></span></button>
  <button class=ib id=bfrz aria-label=freeze>
   <svg id=bfrzi width="22" height="22" viewBox="0 0 24 24" fill="currentColor">
   <rect x="7" y="5" width="4" height="14" rx="1"></rect>
   <rect x="13" y="5" width="4" height="14" rx="1"></rect></svg></button>
 </div>
</div>

<div class=scrim id=scrim></div>
<div class=sheet id=sheet>
 <div class=grab></div>
 <div class=f><label>Contrast</label><input type=range id=pc min=0 max=15 value=2><output id=pcv class=n>2</output></div>
 <div class=f><label>Emissivity</label><input type=range id=em min=0.1 max=1 step=0.01 value=0.95><output id=emv class=n>0.95</output></div>
 <div class=sw><label for=dsp>Noise filter</label><input type=checkbox id=dsp></div>
 <div class=sw><label for=mxc>Mirror horizontal</label><input type=checkbox id=mxc></div>
 <div class=sw><label for=myc>Mirror vertical</label><input type=checkbox id=myc></div>
 <p class=note id=note></p>
 <button class=close id=bclose>Close</button>
</div>

<script>
var U=6,SH=20,SM=2;
var cur=null,hist=[],pal=0,em=0.95,pc=2,dsp=0,frz=0,fn=0,fa=Date.now();
var pts=[],dmm=0,rot=0,mrx=0,mry=0;
var PALNAME=['Ironbow','Rainbow'];
try{rot=+localStorage.tcRot||0;mrx=+localStorage.tcMx||0;mry=+localStorage.tcMy||0}catch(e){}
var v=document.getElementById('v'),vc=v.getContext('2d',{alpha:false});
var b=document.createElement('canvas'),bc=b.getContext('2d');
var big=null,tmp=null,rec=null,chunks=[],t0=0,tmr=null;
var COS=[1,0,-1,0],SIN=[0,1,0,-1];
function E(i){return document.getElementById(i)}
function mk(s){var L=new Uint8Array(768),i,k,a,z,f,t;
 for(i=0;i<256;i++){t=i/255;a=s[0];z=s[s.length-1];
  for(k=0;k<s.length-1;k++)if(t>=s[k][0]&&t<=s[k+1][0]){a=s[k];z=s[k+1];break}
  f=z[0]==a[0]?0:(t-a[0])/(z[0]-a[0]);
  L[i*3]=a[1]+(z[1]-a[1])*f;L[i*3+1]=a[2]+(z[2]-a[2])*f;L[i*3+2]=a[3]+(z[3]-a[3])*f}
 return L}
var P=[mk([[0,0,0,10],[.14,26,10,74],[.3,98,12,120],[.46,178,32,92],[.62,232,80,28],[.78,255,152,0],[.91,255,226,64],[1,255,255,255]]),
mk([[0,10,10,70],[.2,0,40,220],[.4,0,200,210],[.6,20,205,40],[.78,240,225,0],[.92,235,40,20],[1,255,255,255]])];
function em4(x){if(em>=.999)return x;var K=273.15,q=(Math.pow(x+K,4)-(1-em)*Math.pow(295.15,4))/em;
 return q<=0?x:Math.pow(q,.25)-K}
function at(d,x,y){x=x<0?0:x>31?31:x;y=y<0?0:y>23?23:y;return d[y*32+x]}
function despeck(d){
 var out=new Float32Array(768),x,y,i,j,k,n,w=[],m;
 for(y=0;y<24;y++)for(x=0;x<32;x++){
  n=0;for(j=-1;j<=1;j++)for(k=-1;k<=1;k++)w[n++]=at(d,x+k,y+j);
  w.length=9;w.sort(function(a,z){return a-z});m=w[4];i=y*32+x;
  out[i]=Math.abs(d[i]-m)>1.2?m:d[i]}
 return out}
function bx(){
 var W=v.width,H=v.height;
 return {W:W,H:H,rw:(rot&1)?H:W,rh:(rot&1)?W:H,
  sc:(rot&1)?Math.min(W/H,H/W):1}}
function toScr(u,w){
 var g=bx(),px=(u-.5)*g.rw,py=(w-.5)*g.rh;
 if(mrx)px=-px; if(mry)py=-py;
 var c=COS[rot],s=SIN[rot];
 return [g.W/2+g.sc*(c*px-s*py), g.H/2+g.sc*(s*px+c*py)]}
function toImg(X,Y){
 var g=bx(),dx=(X-g.W/2)/g.sc,dy=(Y-g.H/2)/g.sc;
 var c=COS[rot],s=SIN[rot];
 var px=c*dx+s*dy,py=-s*dx+c*dy;
 if(mrx)px=-px; if(mry)py=-py;
 return [px/g.rw+.5, py/g.rh+.5]}
function dist(){
 var row=E('distrow');
 if(!dmm){
  row.classList.add('off');
  E('dv').innerHTML='--.--<u>m</u>';
  E('dgeo').innerHTML='<div>no reading</div><div>out of range</div>';
  return}
 row.classList.remove('off');
 var m=dmm/1000,px=2*m*Math.tan(55/32/2*Math.PI/180)*100,w=2*m*Math.tan(55/2*Math.PI/180)*100;
 E('dv').innerHTML=m.toFixed(2)+'<u>m</u>';
 E('dgeo').innerHTML='<div>1 px &asymp; <b>'+px.toFixed(1)+'</b> cm</div>'+
  '<div>field <b>'+w.toFixed(0)+'</b> cm</div>'}
function got(dv){
 dmm=dv.getUint16(8,true);
 if(frz){dist();return}
 var f=new Float32Array(768),i;
 for(i=0;i<768;i++)f[i]=dv.getInt16(10+i*2,true)/100;
 if(dsp)f=despeck(f);
 hist.push(f);while(hist.length>SM)hist.shift();
 var out=new Float32Array(768),k,s;
 for(i=0;i<768;i++){s=0;for(k=0;k<hist.length;k++)s+=hist[k][i];out[i]=em4(s/hist.length)}
 cur=out;fn++;
 var t=Date.now();if(t-fa>=1000){E('fps').textContent=fn+' fps';fn=0;fa=t}
 dist();draw()}
function draw(){
 if(!cur)return;
 var d=cur,i,w=32*U,h=24*U,x,y,sx,sy,x0,y0,fx,fy;
 if(!big||big.length!=w*h){big=new Float32Array(w*h);tmp=new Float32Array(w*h)}
 for(y=0;y<h;y++)for(x=0;x<w;x++){
  sx=(x+.5)/U-.5;sy=(y+.5)/U-.5;x0=Math.floor(sx);y0=Math.floor(sy);fx=sx-x0;fy=sy-y0;
  var A=at(d,x0,y0),B=at(d,x0+1,y0),C=at(d,x0,y0+1),D=at(d,x0+1,y0+1);
  var p1=A+(B-A)*fx,p2=C+(D-C)*fx;big[y*w+x]=p1+(p2-p1)*fy}
 if(SH>0){var am=SH/100,s2,c2;
  for(y=0;y<h;y++)for(x=0;x<w;x++){
   var xm=x>0?x-1:0,xp=x<w-1?x+1:w-1,ym=y>0?y-1:0,yp=y<h-1?y+1:h-1;
   s2=(big[ym*w+xm]+big[ym*w+x]+big[ym*w+xp]+big[y*w+xm]+big[y*w+x]+
       big[y*w+xp]+big[yp*w+xm]+big[yp*w+x]+big[yp*w+xp])/9;
   c2=big[y*w+x];tmp[y*w+x]=c2+am*(c2-s2)}
  var sw=big;big=tmp;tmp=sw}
 var a=Float32Array.from(d).sort(),lo=a[Math.round(pc*7.68)],hi=a[767-Math.round(pc*7.68)];
 if(hi-lo<1.2){var c=(hi+lo)/2;lo=c-.6;hi=c+.6}
 var span=hi-lo,L=P[pal];
 if(b.width!=w){b.width=w;b.height=h}
 var im=bc.createImageData(w,h),px=im.data,t,q;
 for(i=0;i<w*h;i++){t=(big[i]-lo)/span;t=t<0?0:t>1?1:t;q=((t*255)|0)*3;
  px[i*4]=L[q];px[i*4+1]=L[q+1];px[i*4+2]=L[q+2];px[i*4+3]=255}
 bc.putImageData(im,0,0);
 var dp=Math.min(devicePixelRatio||1,2.5),cw=v.clientWidth||320,ch=v.clientHeight||240;
 if(v.width!=Math.round(cw*dp)){v.width=Math.round(cw*dp);v.height=Math.round(ch*dp)}
 var g=bx();
 vc.fillStyle='#000';vc.fillRect(0,0,g.W,g.H);
 vc.imageSmoothingEnabled=true;
 vc.save();
 vc.translate(g.W/2,g.H/2);
 if(rot)vc.rotate(rot*Math.PI/2);
 vc.scale((mrx?-1:1)*g.sc,(mry?-1:1)*g.sc);
 vc.drawImage(b,-g.rw/2,-g.rh/2,g.rw,g.rh);
 vc.restore();
 var mn=1e9,mx2=-1e9,mi=0,xi=0;
 for(i=0;i<768;i++){if(d[i]<mn){mn=d[i];mi=i}if(d[i]>mx2){mx2=d[i];xi=i}}
 E('hiv').textContent=mx2.toFixed(1);E('lov').textContent=mn.toFixed(1);
 E('l0').textContent=lo.toFixed(1);E('l1').textContent=hi.toFixed(1);
 E('sp').innerHTML=d[12*32+16].toFixed(1)+'<u>&deg;C</u>';
 var rg=E('rp').getContext('2d'),ri=rg.createImageData(256,1);
 for(i=0;i<256;i++){ri.data[i*4]=L[i*3];ri.data[i*4+1]=L[i*3+1];ri.data[i*4+2]=L[i*3+2];ri.data[i*4+3]=255}
 var tc=document.createElement('canvas');tc.width=256;tc.height=1;
 tc.getContext('2d').putImageData(ri,0,0);rg.drawImage(tc,0,0,256,10);
 var kk=Math.min(g.W/320,3);
 vc.lineWidth=1.6*kk;vc.font=(11*kk)+'px ui-monospace,monospace';

 // burnt-in overlay, top left: it ends up in photos and videos too
 var l1=(dmm?(dmm/1000).toFixed(2)+' m':'-- m')+'   '+PALNAME[pal];
 var l2=lo.toFixed(1)+' to '+hi.toFixed(1)+' \u00B0C';
 vc.textBaseline='top';
 var bw=Math.max(vc.measureText(l1).width,vc.measureText(l2).width);
 vc.fillStyle='rgba(0,0,0,.45)';
 vc.fillRect(7*kk,7*kk,bw+10*kk,30*kk);
 vc.fillStyle='rgba(255,255,255,.92)';
 vc.fillText(l1,12*kk,10*kk);
 vc.fillStyle='rgba(255,255,255,.72)';
 vc.fillText(l2,12*kk,23*kk);
 vc.textBaseline='bottom';

 var cc=toScr(.5,.5),cx=cc[0],cy=cc[1];
 vc.strokeStyle='rgba(255,255,255,.95)';vc.beginPath();
 vc.moveTo(cx-9*kk,cy);vc.lineTo(cx-3*kk,cy);vc.moveTo(cx+3*kk,cy);vc.lineTo(cx+9*kk,cy);
 vc.moveTo(cx,cy-9*kk);vc.lineTo(cx,cy-3*kk);vc.moveTo(cx,cy+3*kk);vc.lineTo(cx,cy+9*kk);vc.stroke();
 mark(xi,'rgba(255,138,92,.95)',mx2,kk);mark(mi,'rgba(127,184,255,.95)',mn,kk);
 for(i=0;i<pts.length;i++)mark(pts[i],'rgba(255,255,255,.9)',d[pts[i]],kk)}
function mark(id,col,val,kk){
 var p=toScr(((id%32)+.5)/32,((id/32|0)+.5)/24),x=p[0],y=p[1],s=val.toFixed(1);
 vc.strokeStyle=col;vc.beginPath();vc.arc(x,y,3.4*kk,0,6.3);vc.stroke();
 var w=vc.measureText(s).width,tx=x+6*kk;if(tx+w>v.width)tx=x-6*kk-w;
 if(tx<0)tx=0;
 var ty=y-14*kk;if(ty<0)ty=y+2*kk;
 vc.fillStyle='rgba(0,0,0,.55)';vc.fillRect(tx-2*kk,ty,w+4*kk,14*kk);
 vc.fillStyle=col;vc.fillText(s,tx,ty+12*kk)}
v.onpointerdown=function(e){
 if(!cur)return;
 var r=v.getBoundingClientRect();
 var p=toImg((e.clientX-r.left)/r.width*v.width,(e.clientY-r.top)/r.height*v.height);
 var x=Math.floor(p[0]*32),y=Math.floor(p[1]*24);
 if(x<0||x>31||y<0||y>23)return;
 var id=y*32+x,k=-1,i;
 for(i=0;i<pts.length;i++)if(Math.abs((pts[i]%32)-x)<2&&Math.abs((pts[i]/32|0)-y)<2)k=i;
 if(k>=0)pts.splice(k,1);else if(pts.length<5)pts.push(id);
 draw()};
E('gp').onclick=function(e){var t=e.target;if(t.tagName!='BUTTON')return;
 var ch=this.children,i;for(i=0;i<ch.length;i++)ch[i].className=ch[i]==t?'on':'';
 pal=+t.getAttribute('data-p');if(cur)draw()};
E('pc').oninput=function(){pc=+this.value;E('pcv').textContent=pc;if(cur)draw()};
E('em').oninput=function(){em=+this.value;E('emv').textContent=em.toFixed(2);if(cur)draw()};
E('dsp').onchange=function(){dsp=this.checked?1:0};
function store(){try{localStorage.tcRot=rot;localStorage.tcMx=mrx;localStorage.tcMy=mry}catch(e){}}
E('brot').onclick=function(){rot=(rot+1)&3;store();
 this.classList.toggle('on',rot!=0);if(cur)draw()};
E('mxc').checked=!!mrx;E('myc').checked=!!mry;
E('brot').classList.toggle('on',rot!=0);
E('mxc').onchange=function(){mrx=this.checked?1:0;store();if(cur)draw()};
E('myc').onchange=function(){mry=this.checked?1:0;store();if(cur)draw()};
function sheet(o){E('sheet').classList.toggle('on',o);
 E('scrim').classList.toggle('on',o);E('bcfg').classList.toggle('on',o)}
E('bcfg').onclick=function(){sheet(!E('sheet').classList.contains('on'))};
E('scrim').onclick=function(){sheet(false)};
E('bclose').onclick=function(){sheet(false)};
document.querySelector('.grab').onclick=function(){sheet(false)};
var PAUSE='<rect x="7" y="5" width="4" height="14" rx="1"></rect><rect x="13" y="5" width="4" height="14" rx="1"></rect>';
var PLAY='<path d="M8 5.5 L8 18.5 L19 12 Z"></path>';
E('bfrz').onclick=function(){frz=!frz;this.classList.toggle('on');
 E('bfrzi').innerHTML=frz?PLAY:PAUSE};
function stamp(){var d=new Date(),p=function(n){return(n<10?'0':'')+n};
 return d.getFullYear()+p(d.getMonth()+1)+p(d.getDate())+'-'+p(d.getHours())+p(d.getMinutes())+p(d.getSeconds())}
function save(url,name){var a=document.createElement('a');a.href=url;a.download=name;
 document.body.appendChild(a);a.click();a.remove()}
E('bshot').onclick=function(){
 if(!cur)return;
 save(v.toDataURL('image/png'),'thermal-'+stamp()+'.png');
 v.style.opacity=.25;setTimeout(function(){v.style.opacity=1},90)};
function fmt(s){return Math.floor(s/60)+':'+(s%60<10?'0':'')+(s%60)}
E('bvid').onclick=function(){
 var btn=this;
 if(rec&&rec.state=='recording'){rec.stop();return}
 if(!window.MediaRecorder||!v.captureStream){
  E('note').textContent='This browser cannot record video.';sheet(true);return}
 var st=v.captureStream(15);
 var go=function(){
  var mt='';
  ['video/mp4;codecs=avc1.42E01E','video/mp4;codecs=h264','video/mp4',
   'video/webm;codecs=vp8','video/webm'].forEach(function(m){
   if(!mt&&MediaRecorder.isTypeSupported&&MediaRecorder.isTypeSupported(m))mt=m});
  try{rec=mt?new MediaRecorder(st,{mimeType:mt}):new MediaRecorder(st)}
  catch(e){E('note').textContent='Recording failed: '+e.message;sheet(true);return}
  chunks=[];
  rec.ondataavailable=function(e){if(e.data.size)chunks.push(e.data)};
  rec.onstop=function(){
   st.getAudioTracks().forEach(function(t){t.stop()});
   var bl=new Blob(chunks,{type:rec.mimeType||'video/webm'});
   var ext=(bl.type.indexOf('mp4')>=0)?'mp4':'webm';
   save(URL.createObjectURL(bl),'thermal-'+stamp()+'.'+ext);
   btn.classList.remove('on');E('recbox').classList.remove('on');
   clearInterval(tmr)};
  rec.start();
  btn.classList.add('on');E('recbox').classList.add('on');
  t0=Date.now();E('rect').textContent='0:00';
  tmr=setInterval(function(){
   E('rect').textContent=fmt(Math.floor((Date.now()-t0)/1000))},500)};
 if(navigator.mediaDevices&&navigator.mediaDevices.getUserMedia){
  navigator.mediaDevices.getUserMedia({audio:true}).then(function(ms){
   ms.getAudioTracks().forEach(function(t){st.addTrack(t)});go()})
  .catch(function(){E('note').textContent=
   'Audio unavailable: the microphone requires a secure connection (HTTPS). The video will be silent.';
   go()})}
 else go()};
document.onkeydown=function(e){
 if(e.target.tagName=='INPUT')return;
 var k=e.key.toLowerCase();
 if(k==' '||e.code=='Space'){e.preventDefault();E('bfrz').click()}
 else if(k=='r')E('brot').click();
 else if(k=='p')E('bshot').click();
 else if(k=='v')E('bvid').click();
 else if(k=='s')E('bcfg').click();
 else if(k=='escape')sheet(false)};
function conn(){
 var s=new WebSocket('ws://'+(location.hostname||'192.168.4.1')+':81/');
 s.binaryType='arraybuffer';
 s.onopen=function(){E('led').className='d ok';E('msg').textContent='connected'};
 s.onclose=function(){E('led').className='d no';E('msg').textContent='reconnecting';
  setTimeout(conn,1500)};
 s.onmessage=function(e){if(e.data.byteLength==1546)got(new DataView(e.data))}}
conn();dist();
onresize=function(){v.width=0;if(cur)draw()};
</script></body></html>
)HTML";