#!/usr/bin/env python

import sys,os,getpass

#my defaults
defaultSRM='srm://srm01.ncg.ingrid.pt:8443/srm/managerv2'
#baseUserDir='/cms/store/user/'+getpass.getuser()
#baseUserDir='/cms/store/user/psilva'
baseUserDir=''

#lists directory
def listFiles(userdir,srm=defaultSRM,dumpfile='') :
    cmd = 'lcg-ls'
    cmd += ' -bD srmv2 '+ srm + '?SFN='+userdir
    if(len(dumpfile)>0 ) : cmd += ' > ' + dumpfile
    print cmd
    os.system(cmd)

#clears up the directory
def clearDir(userdir,srm=defaultSRM) :
    listFiles(userdir,srm,'tmp.out')
    
    f = open('tmp.out', 'r')
    for ifile in f:
        cmd = 'lcg-del'
        cmd += ' -lD srmv2 '+ srm + '?SFN='+ifile
        os.system(cmd)

    cmd = 'lcg-del'
    cmd += ' -ldD srmv2 '+ srm + '?SFN='+userdir
    os.system(cmd)

#transfer monitor files
def transferMonitorFiles(userdir,outDir,srm=defaultSRM) :
    listFiles(userdir,srm,'tmp.out')

    f = open('tmp.out', 'r')
    for ifile in f:
        tkns = ifile.split('/')
        ifname = tkns[ len(tkns)-1 ]
        if(ifname.find('Monitor') < 0 ) : continue
        cmd = 'lcg-cp'
        cmd += ' -bD srmv2 '+ srm + '?SFN=' + ifile.strip()
        cmd += ' ' + outDir + '/' + ifname
        os.system(cmd)
    

op=sys.argv[1]
userDir=baseUserDir
if(len(sys.argv)>2) : userDir=userDir+'/'+sys.argv[2]
outDir='/tmp'
if(len(sys.argv)>3) : outDir=sys.argv[3]

if(op=='list') : listFiles(userDir)
if(op=='clear') : clearDir(userDir)
if(op=='transfer') : transferMonitorFiles(userDir,outDir)
