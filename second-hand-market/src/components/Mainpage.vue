<template>
  <div id="mainpage">
    <header>
      <div class="branding">
        <img src="../assets/logo.png" alt="平台 Logo" class="logo" />
        <h1>校园二手物品交易平台</h1>
      </div>
      <div class="search-bar">
        <input type="text" placeholder="搜索您需要的物品..." class="search-input" v-model="searchQuery" />
        <button class="search-button" @click="search">搜索</button>
      </div>
    </header>

    <div class="content">
      <aside class="sidebar">
        <div class="profile">
          <img src="../assets/高木.png" alt="用户头像" class="avatar" />
          <h3>祝你所求有所得</h3>
        </div>
        <nav>
          <router-link to="/profile" class="nav-item">个人中心</router-link>
          <router-link to="/favorites" class="nav-item">收藏夹</router-link>
          <router-link to="/listings" class="nav-item">我的发布</router-link>
          <!-- 论坛链接 -->
          <router-link to="/forum" class="nav-item">论坛</router-link>
        </nav>
      </aside>

      <main class="recommendations">
        <h2>推荐商品</h2>

        <!-- 分类栏 -->
        <div class="category-bar">
          <button 
            v-for="category in categories"
            :key="category"
            :class="{'active': selectedCategory === category}"
            @click="filterByCategory(category)"
          >
            {{ category }}
          </button>
        </div>

        <!-- 商品展示 -->
        <div class="product-grid">
          <router-link
            v-for="item in filteredProducts"
            :key="item.id"
            :to="`/product/${item.id}`"
            class="product-card">
            <img 
              :src="item.img_url
                ? `http://localhost:3000/uploads/${item.img_url.split('/').pop()}` 
                : 'https://via.placeholder.com/200'" 
              alt="商品图片" 
              class="product-image"
            />
            <h3 class="product-title">{{ item.title }}</h3>
            <p class="product-price">{{"商品价格: " + item.price + "元"}}</p>
            <p class="product-description">{{ "商家描述: " + item.desc }}</p>
          </router-link>
        </div>
      </main>
    </div>
  </div>
</template>

<script>
import api from '../api'

export default {
  name: "MainPage",
  data() {
    return {
      searchQuery: '',
      products: [], // 所有商品数据
      filteredProducts: [], // 根据类别过滤后的商品
      categories: ['全部', '电子产品', '家具', '书籍', '运动用品'], // 假设的商品类别
      selectedCategory: '全部', // 默认显示全部商品
    };
  },
  methods: {
    async getlistings() {
      try {
        const response = await api.get('/display');
        this.products = response.data.products;
        this.filteredProducts = this.products; // 初始化时展示所有商品
        console.log("获取商品数据成功", this.products);
      } catch (error) {
        console.error("获取商品数据失败", error);
      }
    },

    // 搜索功能
    search() {
      if (this.searchQuery.trim()) {
        this.$router.push({
          path: '/Results',
          query: { query: this.searchQuery.trim() }
        });
      }
    },

    // 根据选中的类别过滤商品
    filterByCategory(category) {
      this.selectedCategory = category;
      if (category === '全部') {
        this.filteredProducts = this.products;
      } else {
        this.filteredProducts = this.products.filter(item => item.category === category);
      }
    }
  },
  mounted() {
    this.getlistings();  // 页面加载时自动调用接口获取商品数据
  }
};
</script>

<style scoped>
/* 通用样式 */
#mainpage {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  font-family: 'Roboto', sans-serif;
  background-color: #34495e; /* 深灰蓝色背景 */
  color: #ecf0f1; /* 浅灰色文字 */
}

/* 顶部搜索栏 */
header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 1.2rem 2rem;
  background: #1f2a33; /* 深墨绿色背景 */
  color: #ecf0f1;
  border-bottom: 1px solid #2c3e50;
}

.branding {
  display: flex;
  align-items: center;
  gap: 1rem;
}

.logo {
  width: 50px;
  height: 50px;
}

.search-bar {
  display: flex;
  align-items: center;
  gap: 0.8rem;
}

.search-input {
  padding: 0.7rem 1rem;
  border-radius: 30px;
  border: none;
  outline: none;
  width: 300px;
  box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
}

.search-button {
  padding: 0.7rem 1.5rem;
  border-radius: 30px;
  background-color: #16a085;
  color: #ecf0f1;
  font-weight: bold;
  border: none;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

.search-button:hover {
  background-color: #1abc9c;
}

/* 主体部分 */
.content {
  display: flex;
  flex: 1;
  padding: 2rem;
  gap: 2rem;
  justify-content: space-between;
  flex-wrap: wrap;
}

/* 左侧侧边栏 */
.sidebar {
  width: 260px;
  background-color: #2c3e50;
  padding: 1.5rem;
  box-shadow: 2px 0 8px rgba(0, 0, 0, 0.2);
  border-radius: 8px;
}

.profile {
  text-align: center;
  margin-bottom: 2rem;
}

.avatar {
  width: 80px;
  height: 80px;
  border-radius: 50%;
  margin-bottom: 0.5rem;
}

.nav-item {
  display: block;
  padding: 0.8rem 1rem;
  margin-bottom: 0.8rem;
  color: #ecf0f1;
  text-decoration: none;
  border-radius: 12px;
  transition: background-color 0.3s ease;
}

.nav-item:hover {
  background-color: #16a085;
  color: white;
}

/* 推荐区域 */
.recommendations {
  flex: 1;
  padding: 2rem;
  background-color: #34495e;
  border-radius: 8px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
}

.category-bar {
  display: flex;
  gap: 1.2rem;
  margin-bottom: 2rem;
  justify-content: flex-start;
}

.category-bar button {
  padding: 0.6rem 1.2rem;
  background-color: #2c3e50;
  border: 1px solid #34495e;
  border-radius: 25px;
  cursor: pointer;
  color: #ecf0f1;
  transition: all 0.3s ease;
}

.category-bar button.active {
  background-color: #16a085;
  color: white;
}

.product-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
  gap: 1.8rem;
}

.product-card {
  background: #2c3e50;
  border-radius: 15px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  padding: 1.2rem;
  text-align: center;
  transition: transform 0.3s ease, box-shadow 0.3s ease;
  cursor: pointer;
  text-decoration: none;
}

.product-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.2);
}

.product-image {
  width: 100%;
  height: 200px;
  object-fit: cover;
  border-radius: 12px;
  margin-bottom: 1rem;
}

.product-title {
  font-size: 1.3rem;
  margin: 0.5rem 0;
  color: #ecf0f1;
}

.product-price {
  font-size: 1.1rem;
  color: #e74c3c;
  margin: 0.5rem 0;
}

.product-description {
  font-size: 1rem;
  color: #bdc3c7;
  margin-bottom: 1rem;
}
</style>
  