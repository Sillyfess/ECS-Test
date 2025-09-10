"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const World_1 = require("./World");
const Transform_1 = require("./components/Transform");
const Velocity_1 = require("./components/Velocity");
const Health_1 = require("./components/Health");
const Tag_1 = require("./components/Tag");
const MovementSystem_1 = require("./systems/MovementSystem");
const HealthSystem_1 = require("./systems/HealthSystem");
const BoundsSystem_1 = require("./systems/BoundsSystem");
function performanceTest() {
    console.log('===== ECS Performance Test =====');
    console.log('Creating world and systems...');
    const world = new World_1.World();
    world.addSystem(new MovementSystem_1.MovementSystem());
    world.addSystem(new BoundsSystem_1.BoundsSystem());
    world.addSystem(new HealthSystem_1.HealthSystem());
    const ENTITY_COUNT = 10000;
    console.log(`\nCreating ${ENTITY_COUNT} entities...`);
    const startCreation = performance.now();
    for (let i = 0; i < ENTITY_COUNT; i++) {
        const entity = world.createEntity();
        world.addComponent(entity, Transform_1.Transform, new Transform_1.Transform(Math.random() * 2000 - 1000, Math.random() * 2000 - 1000, Math.random() * 2000 - 1000));
        world.addComponent(entity, Velocity_1.Velocity, new Velocity_1.Velocity(Math.random() * 200 - 100, Math.random() * 200 - 100, Math.random() * 200 - 100));
        if (Math.random() > 0.3) {
            world.addComponent(entity, Health_1.Health, new Health_1.Health(Math.random() * 100 + 50));
        }
        if (Math.random() > 0.5) {
            world.addComponent(entity, Tag_1.Tag, new Tag_1.Tag(`entity_${i}`));
        }
    }
    const creationTime = performance.now() - startCreation;
    console.log(`Entity creation completed in ${creationTime.toFixed(2)}ms`);
    console.log(`Average per entity: ${(creationTime / ENTITY_COUNT).toFixed(4)}ms`);
    const stats = world.getStats();
    console.log(`\nWorld Stats:`);
    console.log(`- Total entities: ${stats.entityCount}`);
    console.log(`- Active entities: ${stats.activeEntities}`);
    console.log(`- Systems: ${stats.systemCount}`);
    console.log('\n===== Running Update Loop Test =====');
    const updateIterations = 100;
    const updateTimes = [];
    for (let i = 0; i < updateIterations; i++) {
        const startUpdate = performance.now();
        world.update();
        const updateTime = performance.now() - startUpdate;
        updateTimes.push(updateTime);
    }
    const avgUpdateTime = updateTimes.reduce((a, b) => a + b, 0) / updateTimes.length;
    const minUpdateTime = Math.min(...updateTimes);
    const maxUpdateTime = Math.max(...updateTimes);
    console.log(`\nUpdate Loop Results (${updateIterations} iterations):`);
    console.log(`- Average update time: ${avgUpdateTime.toFixed(2)}ms`);
    console.log(`- Min update time: ${minUpdateTime.toFixed(2)}ms`);
    console.log(`- Max update time: ${maxUpdateTime.toFixed(2)}ms`);
    console.log(`- FPS (based on avg): ${(1000 / avgUpdateTime).toFixed(1)}`);
    console.log('\n===== Component Query Performance =====');
    const queryStart = performance.now();
    const entitiesWithMovement = world.entityManager.getEntitiesWithComponentMask((1n << BigInt(Component_1.Component.getTypeId(Transform_1.Transform))) |
        (1n << BigInt(Component_1.Component.getTypeId(Velocity_1.Velocity))));
    const queryTime = performance.now() - queryStart;
    console.log(`Query for entities with Transform + Velocity:`);
    console.log(`- Found ${entitiesWithMovement.length} entities in ${queryTime.toFixed(2)}ms`);
    console.log('\n===== Memory Test (Entity Destruction) =====');
    const destroyCount = 2000;
    console.log(`Destroying ${destroyCount} random entities...`);
    const activeEntities = world.entityManager.getActiveEntities();
    const destroyStart = performance.now();
    for (let i = 0; i < Math.min(destroyCount, activeEntities.length); i++) {
        const randomIndex = Math.floor(Math.random() * activeEntities.length);
        world.destroyEntity(activeEntities[randomIndex].id);
    }
    const destroyTime = performance.now() - destroyStart;
    console.log(`Destruction completed in ${destroyTime.toFixed(2)}ms`);
    const finalStats = world.getStats();
    console.log(`\nFinal World Stats:`);
    console.log(`- Total entities: ${finalStats.entityCount}`);
    console.log(`- Active entities: ${finalStats.activeEntities}`);
    console.log('\n===== Test Complete =====');
    console.log('The ECS successfully handled 10,000 entities with multiple components and systems!');
}
const Component_1 = require("./Component");
performanceTest();
//# sourceMappingURL=test.js.map