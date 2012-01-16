#!/usr/bin/env python

import sys 
import os 
import xml.dom.minidom, pyexpat 
from xml.dom.minidom import Node

#calls crab
def callCrab(opt,dir='') :
    cmd = 'crab '
    #if(useIOnice) : cmd = 'ionice -c 2 -n 3 crab'
    cmd += '-' + opt + ' '
    if( len(dir)>0 ) : cmd += '-c ' + dir
    os.system(cmd)

#
# convert string into boolean
#
def str2bool(s):
    if s is True or s is False:
        return s
    s = str(s).strip().lower()
    return not s in ['false','f','n','0','']
                

# prints out usage
def usage() :
    print "Usage: sub2grid.py sampleList sampleTag operation=create,createonly,query,kill [workdir=./] [crabtempl=crab.cfg.templ]"

if( len(sys.argv) < 4 ) :
    usage()
    sys.exit(2)

workdir="./"
crabtempl="crab.cfg.templ"
sampleList = sys.argv[1] 
reqTag = sys.argv[2] 
reqOp = sys.argv[3] 
if( len(sys.argv) > 4 ) :
    workdir=sys.argv[4]+"/"
if( len(sys.argv) > 5 ) :
    crabtempl=sys.argv[5]

#parse document and find matches
doc = xml.dom.minidom.parse( sampleList ) 
samples = doc.getElementsByTagName("id") 
for sample in samples :
    
    tag = sample.getAttribute("tag") #working name
    if ( reqTag != 'all' and tag.find( reqTag ) != 0 ) : continue;
    
    gt = sample.getAttribute("gt") #global tag
    ds = sample.getAttribute("ds") #dataset path
    ps = sample.getAttribute("ps") #pset path
    cs = sample.getAttribute("cs") # code sample
    server = sample.getAttribute("server") #crab server
    run = sample.getAttribute("run") #run selection
    encrun = run.replace(",","_")
    eventselection = sample.getAttribute("eventselection")
    eventselection = eventselection.replace(",","','")
    jsonfile = sample.getAttribute("jsonfile")
    splitbyrun = sample.getAttribute("splitbyrun")
    storeoutput = sample.getAttribute("storeoutput")
    if( len(storeoutput) == 0 ) : storeoutput="False"
    fastsim = "False"
    if( str2bool(sample.getAttribute("fastsim")) ): fastsim="True"
    isaod="False"
    if( str2bool(sample.getAttribute("isaod")) ): isaod="True"
    producePDFweights="False"
    if( str2bool(sample.getAttribute("producePDFweights")) ): producePDFweights="True"
    cleanhbhenoise = "True"
    if( str2bool(sample.getAttribute("nohbhenoiseclean") ) ) : cleanhbhenoise="False"
    trigpath=sample.getAttribute("trigpath")
    trigressource=sample.getAttribute('trigressource')
    if(len(trigressource)==0) : trigressource='TriggerResults::HLT'
    outputfile = sample.getAttribute("outputfile")
    ntoprocess=sample.getAttribute("ntoprocess")
    if(len(cs)==0) : cs="DATA"
    if(len(ntoprocess)==0) : ntoprocess="10000"
    totalevents=sample.getAttribute("totalevents")
    if(len(totalevents)==0) : totalevents="-1"
    mutrig=sample.getAttribute("mutrig")
    if(len(mutrig)==0) : mutrig="HLT_Mu12_v1"
    jettrig=sample.getAttribute("jettrig")
    if(len(jettrig)==0) : jettrig="HLT_Jet30_v1"
    egtrig=sample.getAttribute("egtrig")
    if(len(egtrig)==0) : egtrig="HLT_Ele17_CaloIdL_CaloIsoVL_v2"
    minpthat=sample.getAttribute("minpthat")
    if(len(minpthat)==0) : minpthat="-1"
    maxpthat=sample.getAttribute("maxpthat")
    if(len(maxpthat)==0) : maxpthat="14000"
    mindymass=sample.getAttribute("mindymass")
    if(len(mindymass)==0) : mindymass="-1"
    maxdymass=sample.getAttribute("maxdymass")
    if(len(maxdymass)==0) : maxdymass="14000"
    myplugin=sample.getAttribute("myplugin")
    if(len(myplugin)==0) :
        print '*** No Plugin specified (probably not needed)... setting to TopDileptons ***'
        myplugin='TopDileptons'
    
    goodlumis = ''
    goodlumisel = sample.getElementsByTagName("goodlumis")
    if( len(goodlumisel) > 0) : goodlumis = str(goodlumisel[0].childNodes[0].nodeValue)

    badlumis =''
    badlumisel = sample.getElementsByTagName("badlumis")
    if( len(badlumisel) > 0) : badlumis = str(badlumisel[0].childNodes[0].nodeValue)
    
    dbsurl = sample.getAttribute("dbsurl") #dbs url

    print '\033[0;36m' + reqOp + '-ing \033[0m : ' + tag + ' @ ' + ds
    if (reqOp == 'create' or reqOp == 'createonly' ) :
        #create the crab configuration and submit jobs
        cmd = 'sed \"'
        cmd += 's%@cs%' + cs +'%;'
        cmd += 's%@sampledir%' + workdir+tag +'%;'
        cmd += 's%@sampletag%' + tag +'%;'
        cmd += 's%@dataset%' + ds +'%;'
        cmd += 's%@ntoprocess%' + ntoprocess +'%;'
        cmd += 's%@totalevents%' + totalevents +'%;'
        if(len(server)>0) : cmd += 's%@server%use_server = 1%;' #cmd += 's%@server%server_name=' + server +'%;'
        else : cmd += 's%@server% %;'
        if(len(jsonfile)>0) :
		cmd += 's%@lumi_mask%lumi_mask = ' + jsonfile + '%;'
		cmd += 's%events_per_job%lumis_per_job%;'
		cmd += 's%total_number_of_events%total_number_of_lumis%;'
        else : cmd += 's%@lumi_mask% %;'
        if(len(splitbyrun)>0):
            cmd += 's%@split_by_run%split_by_run = 1%;'
        else : cmd += 's%@split_by_run% %;'
        
        if(len(outputfile)>0) : cmd += 's%@output_file%get_edm_output = 1%;'
        else : cmd += 's%@output_file% %;'
        cmd += 's%@parameterset%' +workdir+tag + '_' + ps +'%;'
        cmd += 's%@globaltag%' + gt +'%;'
        cmd += 's%@myplugin%' + myplugin +'%;'
        cmd += 's%@cleanhbhenoise%' + cleanhbhenoise +'%;'
        cmd += 's%@trigressource%' + trigressource + '%;'
        cmd += 's%@mutrig%'+mutrig+'%;'
        cmd += 's%@jettrig%'+jettrig+'%;'
        cmd += 's%@egtrig%'+egtrig+'%;'
        cmd += 's%@minpthat%' + minpthat +'%;'
        cmd += 's%@maxpthat%' + maxpthat +'%;'
        cmd += 's%@mindymass%' + mindymass +'%;'
        cmd += 's%@maxdymass%' + maxdymass +'%;'
        cmd += 's%@fastsim%' + fastsim +'%;'
        cmd += 's%@isaod%' + isaod +'%;'
        cmd += 's%@producePDFweights%' + producePDFweights +'%;'
        cmd += 's%@storeoutput%' + storeoutput +'%;'
        cmd += 's%@trigpath%' + trigpath +'%;'
        cmd += 's%@run%' + run +'%;'
        cmd += 's%@encrun%' + encrun +'%;'
        cmd += 's%@eventselection%'+eventselection+'%;'
        if(len(dbsurl)>0) : cmd += 's%@dbs_url%dbs_url = ' + dbsurl +'%;'
        else : cmd += 's%@dbs_url% %;'
        cmd += 's%@goodlumis%' + goodlumis +'%;'
        cmd += 's%@badlumis%' + badlumis +'%;'
        cmd += '\" '
        os.system(cmd + '< ' + crabtempl + ' > crab.cfg')
        os.system(cmd + '< ' + ps + ' > ' + workdir+tag + '_' + ps)
        callCrab('create')
        os.system('mv ' + workdir + tag + '_' + ps + '* ' + workdir + tag)
        if( reqOp == 'create' ):
            callCrab('submit',workdir+tag)
    elif (reqOp =='query') :
        #query status and submit jobs
        callCrab('status',workdir+tag)
        callCrab('get',workdir+tag)
    elif (reqOp =='kill') :
        #query status and submit jobs
        callCrab('kill all',workdir+tag)
    else :
        usage()
        sys.exit(2)
