#include "ip_hook.h" 
#include "ip_setting.h"
// int ret;
unsigned int IP_HookPreRouting(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *)) 
{
	return NF_ACCEPT;
}

unsigned int IP_HookLocalIn(const struct nf_hook_ops *ops,
			       struct sk_buff* skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *)) 
{
	printk(KERN_INFO"\npacket_in:\n");
	__be32 sip,dip;  
	struct iphdr *iph = NULL;
	iph = ip_hdr(skb);
	sip = iph->saddr; 
	dip = iph->daddr;
	printk("src_ip:"NIPQUAD_FMT"	dst_ip:"NIPQUAD_FMT"\n",NIPQUAD(sip), NIPQUAD(dip)); 
 	if(skb != NULL &&  sip != in_aton(denied_ip) )
 	{ 
		return NF_ACCEPT;
	} 
	if(skb != NULL &&  sip == in_aton(denied_ip))
	{
		printk(KERN_ERR"packetsIN  droped for	src_ip="NIPQUAD_FMT"\n",NIPQUAD(sip));
		return NF_DROP;			
	}

	// check_ip_packets(skb, denied_ip);
	// return NF_ACCEPT;
}

unsigned int IP_HookForward(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *)) 
{
	return NF_ACCEPT;
}
unsigned int IP_HookLocalOut(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *)) 
{
	// int checkID=5;
	printk(KERN_INFO"\npacket_out:\n");
	// check_ip_packets(skb, denied_ip);
	// return NF_ACCEPT;


	__be32 sip,dip;  
	struct iphdr *iph = NULL;
	iph = ip_hdr(skb);
	sip = iph->saddr; 
	dip = iph->daddr;
	printk("src_ip:"NIPQUAD_FMT"	dst_ip:"NIPQUAD_FMT"\n",NIPQUAD(sip), NIPQUAD(dip)); 
 	if(skb != NULL &&  dip != in_aton(denied_ip) )
 	{ 
		return NF_ACCEPT;
	} 
	if(dip == in_aton(denied_ip))
	{
		printk(KERN_ERR"packetsOUT droped for	dst_ip="NIPQUAD_FMT"\n",NIPQUAD(dip));
		return NF_DROP;			
	}
}


unsigned int IP_HookPostRouting(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *)) 
{
	return NF_ACCEPT;
}

/* 钩子挂接结构 */
static struct nf_hook_ops ip_hooks[]  = {
	{
		.hook		= IP_HookLocalIn,		/*本地接收数据*/
		//.owner		= THIS_MODULE,			/*模块所有者*/
		.pf		= PF_INET,			/*网络协议*/
		.hooknum	= NF_INET_LOCAL_IN,		/*挂接点*/
		.priority	= NF_IP_PRI_FILTER-1,	/*优先级*/
	},

	{
		.hook		= IP_HookLocalOut,		/*本地发出的数据*/
		//.owner		= THIS_MODULE,			/*模块所有者*/
		.pf			= PF_INET,				/*网络协议*/
		.hooknum	= NF_INET_LOCAL_OUT,	/*挂接点*/
		.priority	= NF_IP_PRI_FILTER-1,	/*优先级*/
	},
	{
		.hook		= IP_HookForward,		/*本地发出的数据*/
		//.owner		= THIS_MODULE,			/*模块所有者*/
		.pf			= PF_INET,				/*网络协议*/
		.hooknum	= NF_INET_FORWARD,		/*挂接点*/
		.priority	= NF_IP_PRI_FILTER-1,	/*优先级*/
	},
	{
		.hook		= IP_HookPreRouting,	/*本地发出的数据*/
		//.owner		= THIS_MODULE,			/*模块所有者*/
		.pf			= PF_INET,				/*网络协议*/
		.hooknum	= NF_INET_PRE_ROUTING,	/*挂接点*/
		.priority	= NF_IP_PRI_FILTER-1,	/*优先级*/
	},
	{
		.hook		= IP_HookPostRouting,	/*本地发出的数据*/
		//.owner		= THIS_MODULE,			/*模块所有者*/
		.pf			= PF_INET,				/*网络协议*/
		.hooknum	= NF_INET_POST_ROUTING,	/*挂接点*/
		.priority	= NF_IP_PRI_FILTER-1,	/*优先级*/
	},
};

static int __init check_ip_init(void)  
{
	// ret = nf_register_hooks(ip_hooks, ARRAY_SIZE(ip_hooks));
	// if(ret) 
	// {
	// 	printk(KERN_ERR"nf_register_hook() failed\n");
	// 	return -1;
	// }
	nf_register_hooks(ip_hooks, ARRAY_SIZE(ip_hooks));
	printk(KERN_INFO"The Module init successfully\n");
	//printk(KERN_ERR"nf_register_hook() successfully\n");  
	return 0;
}

static int __exit check_ip_exit(void)  
{	
	// ret = nf_unregister_hooks(ip_hooks, ARRAY_SIZE(ip_hooks));
	// if(ret) 
	// {
	// 	printk(KERN_ERR"nf_unregister_hook() failed\n");
	// 	return -1;
	// }
	nf_unregister_hooks(ip_hooks, ARRAY_SIZE(ip_hooks));
	printk(KERN_INFO"The Module exit successfully\n");
	//printk(KERN_ERR"nf_register_hook() successfully\n");  
	return 0;
}

module_init(check_ip_init);
module_exit(check_ip_exit);
MODULE_AUTHOR("zerosun");
MODULE_DESCRIPTION("ip_hook");
MODULE_VERSION("0.0.1");
MODULE_ALIAS("VER 0.0.1");
MODULE_LICENSE("GPL");