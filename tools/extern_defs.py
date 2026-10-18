#!/usr/bin/env python3
"""Convert file-scope definitions of selected globals to extern decls.
Masks comments/strings for detection; edits apply to original text.
"""
import re,sys,glob

ALL = sys.argv[1] == '--all'
if not ALL:
    names=set(l.strip() for l in open(sys.argv[1]) if l.strip() and not l.startswith('#'))
files=[f for f in glob.glob('src/*.[cC]')+glob.glob('h/*.[hH]') if not f.upper().endswith('RPASM.H')]

def sanitize(text):
    """blank /* */ comments and string/char literals, preserving newlines."""
    t=list(text);i=0;n=len(t);st=None
    while i<n:
        c=t[i]
        if st is None:
            if c=='/' and i+1<n and t[i+1]=='*':
                st='comment';t[i]=t[i+1]=' ';i+=2;continue
            if c=='/' and i+1<n and t[i+1]=='/':
                while i<n and t[i]!='\n':t[i]=' ';i+=1
                continue
            if c=='"':st='"';t[i]=' ';i+=1;continue
            if c=="'":st="'";t[i]=' ';i+=1;continue
        elif st=='comment':
            if c=='*' and i+1<n and t[i+1]=='/':
                t[i]=t[i+1]=' ';i+=2;st=None;continue
            if c!='\n':t[i]=' '
        else:
            if c=='\\':t[i]=t[i+1]=' ' if i+1<n else ' ';i+=2;continue
            if (st=='"' and c=='"') or (st=="'" and c=="'"):t[i]=' ';i+=1;st=None;continue
            if c!='\n':t[i]=' '
        i+=1
    return ''.join(t)

def split_decls(stmt):
    parts=[];d=0;cur=''
    for ch in stmt:
        if ch in '([{': d+=1
        elif ch in ')]}': d-=1
        if ch==',' and d==0:
            parts.append(cur);cur=''
        else: cur+=ch
    parts.append(cur)
    return parts

report={}
for fn in files:
    try: text=open(fn,errors='replace').read()
    except: continue
    clean=sanitize(text)
    # work on character positions: find depth-0 statements in clean text
    out=[];pos=0;n=len(text);depth=0;changed=False;krpend=False
    while pos<n:
        # advance to next statement boundary at depth 0
        if depth!=0 or clean[pos].isspace() or clean[pos]=='\n':
            if clean[pos]=='{':depth+=1
            elif clean[pos]=='}':depth-=1
            out.append(text[pos]);pos+=1;continue
        if clean[pos]=='#':
            # skip whole preprocessor line
            while pos<n and text[pos]!='\n':
                out.append(text[pos]);pos+=1
            continue
        # at depth 0, non-newline: capture statement = until ';' or '{' at rd0 (clean text)
        j=pos;rd=0;term=None
        while j<n:
            c=clean[j]
            if c=='\n' and j+1<n and clean[j+1:j+2]=='#':
                term='pp';break
            if c=='{':
                if rd==0:term='brace';break
                rd+=1
            elif c=='}':
                if rd==0:term='rbrace';break  # statement starting with } (struct close decl)
                rd-=1
            elif c==';' and rd==0:term='semi';break
            j+=1
        if term is None:
            out.append(text[pos:]);break
        if term=='pp':
            out.append(text[pos:j]);pos=j;continue
        if term=='brace':
            head=clean[pos:j]
            if not head.strip():
                out.append(text[pos:j+1]);pos=j+1;depth=1;krpend=False;continue
            if '(' in head:
                # function def: emit verbatim, account depth via clean braces
                out.append(text[pos:j+1]);pos=j+1
                # continue loop at depth 1 (the { counted)
                depth=1
                changed=False or changed
                continue
            # struct decl or array-init: keep scanning to ';' at rd0
            jj=j;rd=0
            while jj<n:
                c=clean[jj]
                if c=='{':rd+=1
                elif c=='}':rd-=1
                elif c==';' and rd==0:break
                jj+=1
            stmt=clean[pos:jj+1];term='semi';j=jj
        elif term=='rbrace':
            if j==pos:  # bare } at depth 0 (unbalanced region) - emit verbatim
                out.append(text[pos]);pos+=1;continue
            # 'struct T{...}' decl where } closes type body; capture to ';' at rd0
            jj=j;rd=0
            while jj<n:
                c=clean[jj]
                if c=='{':rd+=1
                elif c=='}':rd-=1
                elif c==';' and rd==0:break
                jj+=1
            stmt=clean[pos:jj+1];term='semi';j=jj
        else:
            stmt=clean[pos:j+1]
        st=stmt.strip()
        par=st.find('(');eq=st.find('=')
        newkr = par>=0 and (eq<0 or par<eq)
        if newkr:
            rp=st.rfind(')')
            krpend = krpend or (bool(st[rp+1:].rstrip(';').strip()) if rp>=0 else True)
        if krpend or newkr:
            out.append(text[pos:j+1]);pos=j+1
            continue
        # process decl: must contain a target name and look like a def
        m=re.search(r'\b('+'|'.join(re.escape(x) for x in sorted(names,key=len,reverse=True))+r')\b',st) if not ALL else True
        if (not m) or 'extern' in st or re.match(r'^(typedef|return|if|while|for|switch|else|do)\b',st) or (ALL and re.search(r'\bstatic\b',st)):
            out.append(text[pos:j+1])
            for ch in stmt:
                if ch=='{':depth+=1
                elif ch=='}':depth-=1
            pos=j+1;continue
        body=st[:-1] if st.endswith(';') else st
        mstruct=re.match(r'^((?:struct|union|enum)\s+\w*\s*\{.*?\})\s*(.*)$',body,re.S)
        structpart=None
        if mstruct:structpart=mstruct.group(1);body2=mstruct.group(2)
        else:body2=body
        if '=' not in body2 and not re.search(r'\w+\s*(\[[^\]]*\])?\s*$',body2):
            out.append(text[pos:j+1])
            for ch in stmt:
                if ch=='{':depth+=1
                elif ch=='}':depth-=1
            pos=j+1;continue
        parts=split_decls(body2)
        mt=re.match(r'^(.*?)([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)\s*(=|$)',parts[0],re.S)
        if not mt:
            out.append(text[pos:j+1]);pos=j+1;continue
        typepfx=mt.group(1).strip()
        externs=[];keeps=[];hitany=False;firstkept=False
        for k,p in enumerate(parts):
            dm=re.search(r'([A-Za-z_]\w*)\s*((?:\[[^\]]*\])*)\s*(=.*)?$',p.strip(),re.S)
            if not dm:keeps.append((k==0,p.strip()));continue
            vname=dm.group(1);dims=dm.group(2) or ''
            tsw=typepfx.split()
            if tsw and tsw[-1] in ('struct','union','enum'):
                keeps.append((k==0,p.strip()));continue
            if vname in ('struct','union','enum','const','far','near','unsigned','signed','void','volatile','cdecl','pascal','fortran','huge','interrupt'):
                keeps.append((k==0,p.strip()));continue
            if ALL or vname in names:
                hitany=True
                tp=typepfx
                if structpart:
                    mtag=re.match(r'(struct|union|enum)\s+(\w+)',structpart)
                    tp=mtag.group(1)+' '+mtag.group(2) if mtag else tp
                tp=re.sub(r'\bstatic\b','',tp).strip()
                externs.append('extern %s %s%s;' % (tp,vname,dims))
                report.setdefault(vname,fn)
            else:
                keeps.append((k==0,p.strip()))
        if not hitany:
            out.append(text[pos:j+1])
            for ch in stmt:
                if ch=='{':depth+=1
                elif ch=='}':depth-=1
            pos=j+1;continue
        newl=''
        if structpart:newl+=structpart+';\n'
        newl+='\n'.join(externs)+'\n'
        if keeps:
            if not keeps[0][0]: newl+=typepfx+' '
            newl+=keeps[0][1]
            for kp in keeps[1:]:newl+=','+kp[1]
            newl+=';\n'
        out.append(newl)
        for ch in stmt:
            if ch=='{':depth+=1
            elif ch=='}':depth-=1
        pos=j+1;changed=True
    if changed:
        open(fn,'w').write(''.join(out))
        print('EDITED',fn)
for nm in sorted(names) if not ALL else []:
    if nm not in report: print('# NO-SITE',nm)
