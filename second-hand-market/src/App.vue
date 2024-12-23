<template>
  <div id="app">
    <!-- 按钮切换侧边栏显示/隐藏 -->
    <button @click="toggleSidebar" class="sidebar-toggle">☰</button>

    <!-- 侧边栏 -->
    <aside class="sidebar" v-show="isSidebarVisible">
      <div class="profile">
        <img src="./assets/高木.png" alt="用户头像" class="avatar" />
        <h3>祝你所求有所得</h3>
      </div>
      <nav>
        <router-link to="/Mainpage" class="nav-item">返回主页</router-link>
        <router-link to="/profile" class="nav-item">个人中心</router-link>
        <router-link to="/favorites" class="nav-item">收藏夹</router-link>
        <router-link to="/listings" class="nav-item">我的发布</router-link>
        <router-link to="/forum" class="nav-item">论坛</router-link>
      </nav>
    </aside>

    <!-- 主内容区域 -->
    <main :style="{ marginLeft: isSidebarVisible ? '250px' : '0' }">
      <router-view></router-view> <!-- This renders the content based on the current route -->
    </main>
  </div>
</template>

<script>
export default {
  name: "MainApp",
  data() {
    return {
      // 控制侧边栏显示/隐藏的状态
      isSidebarVisible: false,
    };
  },
  methods: {
    // 切换侧边栏的显示状态
    toggleSidebar() {
      this.isSidebarVisible = !this.isSidebarVisible;
    },
  },
};
</script>

<style scoped>
/* 侧边栏的样式 */
.sidebar {
  width: 250px;
  position: fixed;
  top: 0;
  left: 0;
  height: 100%;
  background-color: #f4f4f4;
  padding: 20px;
  box-shadow: 2px 0 5px rgba(0, 0, 0, 0.1);
  transition: transform 0.3s ease; /* 添加动画 */
}

.profile {
  text-align: center;
  margin-bottom: 20px;
}

.profile .avatar {
  width: 50px;
  height: 50px;
  border-radius: 50%;
}

nav .nav-item {
  display: block;
  margin: 10px 0;
  padding: 10px;
  text-decoration: none;
  color: #333;
}

nav .nav-item:hover {
  background-color: #ddd;
}

main {
  padding: 20px;
  transition: margin-left 0.3s ease; /* 添加动画 */
}

/* 切换按钮的样式 */
.sidebar-toggle {
  position: fixed;
  top: 20px;
  left: 20px;
  background-color: #fff; /* 橙色背景 */
  color: #fff;
  border: none;
  padding: 12px 16px; /* 增加内边距 */
  cursor: pointer;
  font-size: 24px;
  border-radius: 50%; /* 圆形按钮 */
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1); /* 添加阴影 */
  transition: all 0.3s ease; /* 平滑过渡效果 */
  z-index: 1000;
}

.sidebar-toggle:hover {
  background-color: #fff; /* 悬停时的颜色 */
  transform: scale(1.1); /* 按钮放大 */
}

.sidebar-toggle:focus {
  outline: none;
}

.sidebar-toggle:active {
  transform: scale(0.95); /* 按钮按下时缩小 */
}

</style>
