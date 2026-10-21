import subprocess,re,sys,tempfile,os
def img(p):
    d=open(p,'rb').read(); h=int.from_bytes(d[8:10],'little')*16
    pg=int.from_bytes(d[4:6],'little'); lp=int.from_bytes(d[2:4],'little')
    return d[h:(pg-1)*512+(lp or 512)]
G=img('/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE')
M=img('build/obj/F14.EXE')
segmap={};fn={}
for l in open('build/f14_named.map'):
    m=re.match(r'(\w+) CODE (\w+)',l)
    if m: segmap[m.group(1)]=int(m.group(2),16)
    m=re.match(r'(\w+): (\w+) FAR (\w+)-(\w+)',l)
    if m: fn[m.group(1)]=(m.group(2),int(m.group(3),16),int(m.group(4),16))
anchors={}
for l in open('analysis/gold/verify_current.txt'):
    f=l.split()
    if len(f)>3 and f[1]=='DIFF':
        anchors[f[0]]=f[2][1:]
def dis(img,off,n=64):
    with tempfile.NamedTemporaryFile(delete=False) as t:
        t.write(img[off:off+n*4]);p=t.name
    o=subprocess.run(['ndisasm','-b16','-o'+hex(off),p],capture_output=True).stdout.decode()
    os.unlink(p)
    out=[]
    for l in o.splitlines():
        m=re.match(r'([0-9A-Fa-f]{8})\s+(\S+)\s+(.*)',l)
        if m: out.append(m.group(3).strip())
        if len(out)>=n: break
    return out
for name in sys.argv[1:]:
    if name not in anchors or name not in fn: print(name,'MISSING'); continue
    gs,go=anchors[name].split(':')
    ga=int(gs,16)*16+int(go,16)
    mod,a,b=fn[name]; ca=segmap[mod]*16+a
    gl=dis(G,ga); cl=dis(M,ca)
    print('=== %s gold@%x cand@%x(%s:%x) ==='%(name,ga,ca,mod,a))
    ndiff=0
    for i in range(min(len(gl),len(cl))):
        x,y=gl[i],cl[i]
        tag='  ' if x==y else '!!'
        if tag=='!!': ndiff+=1
        print('%s %3d G:%-38s C:%s'%(tag,i,x[:38],y[:38]))
        if i>50: break
    print('   first-diffs shown; total-ish',ndiff)
