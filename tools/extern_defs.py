#!/usr/bin/env python3
"""Convert file-scope definitions of selected globals to extern decls.
Masks comments/strings for detection; edits apply to original text.
"""
import re,sys,glob

names=set(l.strip() for l in open(sys.argv[1]) if l.strip() and not l.startswith('#'))
files=glob.glob('src/*.[cC]')+glob.glob('h/*.[hH]')

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
    out=[];pos=0;n=len(text);depth=0;changed=False
    while pos<n:
        # advance to next statement boundary at depth 0
        if depth!=0 or clean[pos].isspace() or clean[pos]=='\n':
            ch=text[pos]
            if ch=='{':depth+=1
            elif ch=='}':depth-=1
            out.append(ch);pos+=1;continue
        if clean[pos]=='#':
            # skip whole preprocessor line
            while pos<n and text[pos]!='\n':
                out.append(text[pos]);pos+=1
            continue
        # at depth 0, non-newline: capture statement = until ';' or '{' at rd0 (clean text)
        j=pos;rd=0;term=None
        while j<n:
            c=clean[j]
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
        if term=='brace':
            head=clean[pos:j]
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
        # process decl: must contain a target name and look like a def
        m=re.search(r'\b('+'|'.join(re.escape(x) for x in sorted(names,key=len,reverse=True))+r')\b',st)
        if (not m) or 'extern' in st or re.match(r'^(typedef|return|if|while|for|switch|else|do)\b',st):
            out.append(text[pos:j+1])
            for ch in stmt:
                if ch=='{':depth+=1
                elif ch=='}':depth-=1
            pos=j+1;continue
        body=st[:-1] if st.endswith(';') else st
        mstruct=re.match(r'^(struct\s+\w+\s*\{[^{}]*\})\s*(.*)$',body,re.S)
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
        mt=re.match(r'^(.*?)([A-Za-z_]\w*)\s*(\[[^\]]*\])*\s*(=|$)',parts[0],re.S)
        if not mt:
            out.append(text[pos:j+1]);pos=j+1;continue
        typepfx=mt.group(1).strip()
        externs=[];keeps=[];hitany=False;firstkept=False
        for k,p in enumerate(parts):
            dm=re.search(r'([A-Za-z_]\w*)\s*(\[[^\]]*\])*\s*(=.*)?$',p.strip(),re.S)
            if not dm:keeps.append((k==0,p.strip()));continue
            vname=dm.group(1);dims=dm.group(2) or ''
            if vname in names:
                hitany=True
                tp=typepfx
                if structpart:
                    mtag=re.match(r'struct\s+(\w+)',structpart)
                    tp='struct '+mtag.group(1) if mtag else tp
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
for nm in sorted(names):
    if nm not in report: print('# NO-SITE',nm)
