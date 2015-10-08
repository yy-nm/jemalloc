## jemalloc 
- github 上默认的配置参数如下:
	+ export MALLOC_CONF="prof:true,lg_prof_sample:1,prof_accum:false,prof_prefix:jeprof.out"
	+ /etc/malloc.conf
	+ 由于真实项目应用下不存在 malloc.conf 或者 MALLOC_CONF 环境变量的设置, 所以使用 malloc 默认方式
- 入手的地方
	+ 通过查找其他开源项目调用的函数
		* skynet
		* ~~rethinkdb~~
		* redis
	+ 通过 github 上 jemalloc 的 Getting Started 介绍入手
- 涉及到的调用函数如下
	+ je_malloc_usable_size
	+ je_malloc_stats_print
	+ je_malloc
	+ je_realloc
	+ je_free
	+ je_mallctl
	+ je_calloc
- 一些 linux 系统函数
	+ atexit: 注册函数, 当程序终止时会被调用的函数， 最多能设置 32 个函数
	+ sbrk: 增加程序数据空间, 当输入为 0 时, 返回当前已经分配的内存大小
	+ pthread_atfork: 注册函数用于当前线程执行 fork 时进行回调, 第一个参数在 fork 之前被调用, 第二个参数在 fork 之后在父进程中被调用, 第三个参数在 fork 之后在子进程中被调用
- jemalloc 真正用于内存分配的函数
	+ sbrk
	+ mmap: chunk(chunk_alloc_mmap), 
		* mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
			- MAP_ANON: 申请的内存已经被初始化了

### jemalloc 调用情况
- je_malloc: 分配内存
	+ imalloc_body: 初始化并对 size 进行处理
		* malloc_init: 初始化状态以及申请内存线程初始化
			- malloc_initialized: 比较 jemalloc 内存分配器状态
			- malloc_init_hard: 加锁, 初始化必要的数据
				+ malloc_init_hard_needed: 主要是防止初始化的时候进行申请, 通过让其不停等待
				+ malloc_init_hard_a0_locked: 设置内存分配线程 id, pthread_self, 并初始化 malloc_conf 配置
					* malloc_initializer = INITIALIZER;
					* malloc_conf_init: 该函数基本上没有用处
					* base_boot: 初始化 jemalloc 要使用到的 红黑树
						- extent_tree_szad_new: 由 rb_gen 宏定义生成出来, 用于生成红黑树
					- chunk_boot: 创建 chunk
						+ opt_lg_chunk = 21;
						+ chunksize = 2^21, 2M
						+ chunksize_mask = 2^21 - 1
						+ chunk_npages = 2^9
						+ chunk_dss_boot: 
							* chunk_dss_sbrk: 使用 sbrk(0) 来获得当前分配内存的位置
							* dss_base: 当前程序的 Data Segment 的位置
							* dss_prev = dss_base
							* dss_max = dss_base
						- rtree_new: 初始化 R-Tree
					- ctl_boot: 
						- ctl_initialized = false;
					- arena_boot: 
						+ arena_lg_dirty_mult_default_set:
							* arena_lg_dirty_mult_valid: 判断传入的值是否在 [-1, 64) 区间, 由于入参是 3所以返回值为 true
							* atomic_write_z: 将 lg_dirty_mult_default 设置为 3, 此操作是原子操作
							* map_bias = 5
							* map_misc_offset = 2^12 + 2^6
							* arena_maxrun = 2^21 - 2^12 - 2^14
							* arena_maxclass = 7 << 18 = 2^21 - 2^18
							* nlclasses = 39
							* nhclasses = 161
						- bin_info_init: 初始化 arena_bin_info 数组
							+ bin_info_run_size_calc
								* bin_info->redzone_size = 0;
								* // run_size 必须是 page 的整数倍且小于 2M
								* // 必须是 reg_size 的倍数
								* bin_info->run_size = actual_run_size;
								* bin_info->nregs = bin_info->run_size / bin_info->reg_size
								* bin_info->reg0_offset = 0
								* small_maxrun = arena_maxrun = 2^21 - 2^12 - 2^14
							+ bitmap_info_init
								* bitmap_info
								* 以 64 为一层, size_t 全覆盖
								* 最多分 3 层
						- small_run_size_init
							+ base_alloc
								* size = 2^9 - 1 - 4
								* CACHELINE_CEILING: 返回64的倍数, 最少返回 1, 除非输入为0, (x /64 + !!(x % 64)) * 64
								* csize = 2^9
								* s2u:
									- s2u_lookup
										+ size2index_lookup: 16
										+ index2size_lookup: 2^9
								- usize = 2^9
								* extent_node_init(&key, NULL, NULL, usize, false, false);
									- extent_node_arena_set
									- extent_node_addr_set
									- extent_node_size_set
									- extent_node_zeroed_set
									- extent_node_committed_set
									- extent_node_achunk_set
								* node = extent_tree_szad_nsearch(&base_avail_szad, &key);:
								* node = NULL;
								* node = base_chunk_alloc(2^9);:
									- base_node_try_alloc: return NULL;
									- CACHELINE_CEILING
									- nsize = 2 * 64 = 128
									- CHUNK_CEILING
									- csize = 2^21
									- chunk_alloc_base(2^21);
										+ chunk_alloc_mmap(2^21, 2^21, &true, &true);
											* pages_map: 返回 mmap 回的内存
											- ALIGNMENT_ADDR2OFFSET: 对齐, 一般情况都会出现不对齐的问题
											- 如果没有对齐
												+ pages_unmap
												+ chunk_alloc_mmap_slow(2^21, 2^21, &true, &true)
													* 先申请 2M + 2M - 4K 内存, 然后去掉首尾, 获得地址与 2M 对齐的内存块
									- 头部用于 extent_node_t 结构
									- en_addr -> &extent_node_t + 128
									- en_size = 2M - 128
									- en_committed = true
									- en_zeroed = true
									- extent_tree_szad_insert
								- extent_node_addr_get
								- extent_node_size_get
								- extent_node_addr_set
								- extent_node_size_set
							- 多层宏定义用于初始化 arena_bin_info_t 节点
							- small_run_tab[arena_bin_info[index].run_size >> LG_PAGE] = true
							- index: 0 -> 35
					- tcache_boot
						+ tcache_maxclass = 64
						+ size2index
							- size2index_compute
								+ x = 11
								+ shift = 5
								+ grp = 20
								+ lg_delta = 8
								+ delta_inverse_mask = 0xfffffff0
								+ mod = 3
								+ index = 1 + 20 + 3 = 24
						- base_alloc(18 * 32)
							+ csize = 18 * 32 = 9 * 64
							+ usize = 5 * 2^7
							+ base_chunk_alloc(13 * 64);
					- malloc_mutex_init: 初始化 lock 成功返回 false
					- narenas_total = narenas_auto = 1;
					- arenas = &a0;
					- memset
					- arena_init(0):
						+ arena_init_locked(0)
							* arena = arenas[ind];
							* arena == NULL
							* arena_new(0)
								- sizeof(arena_t) = 6800
								- sizeof(malloc_large_stats_t) = 32
								- sizeof(malloc_huge_stats_t) = 24
								- base_alloc(41024)
								- arena->ind = 0;
								- arena->nthreads = 0;
								- memset(arena->stats)
								- 
								- chunk_dss_prec_get
									+ ret = dss_prec_default;
								- arena->dss_prec = dss_prec_secondary
								- arena->spare = NULL;
								- arena_lg_dirty_mult_default_get
									+ return lg_dirty_mult_default
								- arena->lg_dirty_mult = 3
								- arena->purging = false;
								- arena->nactive = 0;
								- arena->ndirty = 0;
								- arena->chunks_szad_cached 创建红黑树
								- arena->chunks_ad_cached 创建红黑树
								- arena->chunks_szad_retained 创建红黑树
								- arena->chunks_ad_retained 创建红黑树
								- arena->node_cache.qlh_first = NULL
								- areba->chunk_hooks = chunk_hooks_default
								- areba->bins[].runs 创建红黑树
					- malloc_init_state = malloc_init_a0_initialized;
				+ malloc_tsd_boot0
					* tsd_boot0
						- a_cleanup = tsd_cleanup
						- a_initializer = TSD_INITIALIZER
						- 给 tsd_tsd 创建 pthread_key, 并制定线程退出函数, tsd_cleanup
					* tsd_fetch()
						- tsd_get: 即返回 tsd_tls(tsd_t 类型)
						- tsd->state = tsd_state_nominal;
						- tsd_set(tsd);
							+ 将 tsd_tls 设置为 tsd_tsd 的 value 值
					* tsd_arenas_cache_bypassp_get: 
						* 本质上是将 tsd_tls->arenas_cache_bypass = true
				+ malloc_init_hard_recursible
					* malloc_init_state = malloc_init_recursible;
					* malloc_mutex_unlock(&init_lock);
					* malloc_ncpus
						- sysconf(_SC_NPROCESSORS_ONLN); // 获得 cpu 核数
						- ncpus = 当前设备 cpu 数
					- pthread_atfork(jemalloc_prefork, jemalloc_postfork_parent, jemalloc_postfork_child): 这三个函数基本是在当前分配内存的线程进行 fork 时才会被使用到, 所以现在不考虑这部分
					- malloc_mutex_lock(&init_lock);
				+ malloc_init_hard_finish
					* base_alloc
					* 为 arenas 分配 2M 内存块
					* memset
					* malloc_init_state = malloc_init_initialized;
				+ malloc_tsd_boot1
					* tsd_boot1: 空函数
					* tsd_arenas_cache_bypassp_get: 本质上是将 tsd_tls->arenas_cache_bypass = false
			- malloc_thread_init
				+ quarantine_alloc_hook: 正常情况下是不会被调用的
		* tsd_fetch
		* s2u: 转化输入的 size, 用于函数返回, 通过指针的形式
			- s2u_lookup: size <= 4k, 才会被调用
				+ size2index_lookup
				+ index2size_lookup
			- s2u_compute
			- usize = 小于 4k, 查表(获得离最近的预分配值), 大于则将零头补齐至 2^4, 即 16
		* imalloc(*tsd, size)
			- tcache_get
				+ tsd_tcache_get: 即 tsd->tcache
				+ 第一次线程申请必为 NULL
					* tcache_get_hard
						- tcache_enabled_get
							- tsd_tcache_enabled_get
							- tsd_tcache_enabled_set
						- arena_choose(tsd, NULL);
							+ tsd_arena_get
							+ arena_choose_hard: 第一次必须执行
								* arena_init_locked: 若没有找到对应 arena 就生成一个
									- 这个函数调用不会出现要扩展 arenas 数组的情况
									- arena_new
						- tcache_create
							- sa2u
							- ipallocztm(tsd, size, CACHELINE, true, false, true, a0get());
								- arena_palloc
									+ arena_palloc(tsd, a0get(), size, CACHELINE, zero, tcache);
										* 使用 small 分配机制
										* arena_malloc
											- arena_malloc_small
												+ arena->bin[]->runcur == null
												+ arena_bin_malloc_hard
													* arena_bin_nonfull_run_get
														- arena_bin_nonfull_run_tryget
															+ arena_bin_runs_first
																* 从 bins[]->runs 红黑树中获取一个值, 
																* 此例中返回 NULL
															+ 返回 NULL
														- arena_run_alloc_small
															+ size 最小 4k, 最大 4k * 7 = 28k
															+ arena_run_alloc_small_helper
																* 找到大小最合适的 chunk
																* arena_run_first_best_fit
																	- run_quantize_first
																		+ small_run_tab[size >> LG_PAGE] 必为 true, 
																	- 获得的大小等于要求的大小
																* arena->runs_avail 是一个空的红黑树, 所以返回 NULL
														- arena_chunk_alloc(arena); 申请空的 chunk
															+ arena->spare == NULL
															+ arena_chunk_init_hard(arena);
																* arena_chunk_alloc_internal
																	- chunk_alloc_cache(arena, &chunk_hooks, NULL, chunksize, chunksize, zero, true);
																		+ chunk_hooks_assure_initialized
																			* 初始化 chunk_hooks=arena->chunk_hooks
																		+ chunk_recycle(arena, chunk_hooks, &arena->chunks_szad_cached, &arena->chunks_ad_cached, true, new_addr, size, alignment, zero, &commit, dalloc_node);
																			* 由于 &arena->chunks_szad_cached 是棵空树, 所以返回 NULL
																		+ NULL
																	- arena_chunk_alloc_internal_hard(arena, &chunk_hooks, zero, commit);
																		+ chunk_alloc_wrapper(arena, chunk_hooks, NULL, chunksize, chunksize, zero, commit);
																			* chunk_hooks->alloc(new_addr, size, alignment, zero, commit, arena->ind);
																			* 本质上调用的是 chunk_alloc_default 函数
																				- chunk_arena_get(1)
																					+ arena_get(tsd_fetch(), 1, false, true);
																						* arena_get_hard
																							- a0malloc
																								+ a0ialloc(size, false, true)
																									* iallocztm(NULL, size, zero, NULL, is_metadata, a0get())
																										- arena_malloc
																											+ arena_malloc_small
																												* arena_bin_malloc_hard
								- iaalloc
								- isalloc
								- arena_metadata_allocated_add
							- tcache_arena_associate
					* tsd_tcache_set: tsd->tcache = tcache
			- iallocztm
				+ - arena_malloc
				+ iaalloc
					* arena_aalloc
						- CHUNK_ADDR2BASE
						- extent_node_arena_get
						- huge_aalloc: 若上面相等就不会执行这步
							+ huge_node_get
								* chunk_lookup
									- rtree_get: R-Tree 中获得节点
							+ extent_node_arena_get
				+ isalloc
					* arena_salloc
						- CHUNK_ADDR2BASE
						- arena_mapbits_allocated_get
						- arena_mapbits_binind_get
						- arena_mapbits_large_get
						- arena_mapbits_large_size_get
						- arena_mapbits_dirty_get
						- arena_mapbits_large_get
						- arena_mapbits_get
						- arena_ptr_small_binind_get
						- index2size
							+ index2size_lookup
						- huge_salloc
							+ huge_node_get
							+ extent_node_arena_get
							+ extent_node_size_get
				- arena_metadata_allocated_add
					+ atomic_add_z
	- isalloc
	- *tsd_thread_allocatedp_get(tsd) += usize; *(tsd->thread_allocated) += usize
	- done
- je_free: 
	- UTRACE: 需要特定参数进行开启, 默认什么都不做
	- tsd_fetch
	- tcache_get
	- ifree
		+ isalloc
		+ tsd_thread_deallocatedp_get: tsd->thread_deallocated
		+ iqalloc
			* idalloctm
				- iaalloc
				- isalloc
				- arena_metadata_allocated_sub
				- arena_dalloc
- je_realloc(void *ptr, size_t size)
	- size=0 相当于 je_free
	- malloc_thread_init: 若不使用隔离技术, 该函数不做任何事情
	- tsd_fetch
	- isalloc
	- s2u
	- irealloc_prof
	- iralloc
	- tcache_get
	- iralloc
		+ iralloct
			* arena_ralloc
				- arena_ralloc_no_move
					+ arena_ralloc_large
						* s2u
						* arena_ralloc_junk_large
						* arena_ralloc_large_shrink
						* arena_ralloc_large_grow
							- arena_mapbits_large_size_get
							- arena_mapbits_allocated_get
							- arena_mapbits_unallocated_size_get
							- size2index
							- index2size
							- arena_miscelm_get
							- arena_run_split_large
							- malloc_mutex_unlock
							- arena_mapbits_dirty_get
							- arena_mapbits_dirty_get
							- arena_mapbits_large_set
							- arena_mapbits_unzeroed_get
							- arena_mapbits_large_set
							- arena_mapbits_unzeroed_get
							- size2index
							- size2index
						- memset
					+ huge_ralloc_no_move, 跟前个函数只能二选一执行
						* s2u
						* huge_ralloc_no_move_similar
							- huge_node_get
							- chunk_purge_wrapper
							- extent_node_size_set
							- extent_node_zeroed_set
							- arena_chunk_ralloc_huge_similar
							- memset
						- huge_ralloc_no_move_shrink
							+ huge_node_get
							+ extent_node_arena_get
							+ chunk_hooks_get
							+ chunk_purge_wrapper
							+ extent_node_size_set
							+ extent_node_zeroed_set
							+ arena_chunk_ralloc_huge_shrink
								* arena_huge_ralloc_stats_update
									- arena_huge_dalloc_stats_update
									- arena_huge_malloc_stats_update
									- stats_cactive_sub
										+ atomic_sub_z
									- chunk_dalloc_cache
										+ chunk_record
										+ arena_maybe_purge
							- huge_ralloc_no_move_expand
							- huge_ralloc_no_move_expand
								+ s2u
								+ huge_node_get
								+ extent_node_zeroed_get
								+ arena_chunk_ralloc_huge_expand
				- sa2u
				- ipalloct
					+ ipallocztm
				- arena_malloc
					+ arena_choose
					+ tcache_alloc_small
					+ arena_malloc_small
					+ tcache_alloc_large
					+ arena_malloc_large
					+ huge_malloc
				- memcpy
				- isqalloc
					+ isdalloct
					+ arena_sdalloc
				- huge_ralloc
	- imalloc_body
	- tsd_thread_allocatedp_get
	- tsd_thread_deallocatedp_get
- je_calloc(size_t num, size_t size)
	+ malloc_init
	+ tsd_fetch
	+ s2u
	+ icalloc
		* tcache_get
		* iallocztm
		* isalloc
		* tsd_thread_allocatedp_get
- je_malloc_stats_print(0,0,0);
	+ stats_print
		* je_mallctl
- je_mallctl
