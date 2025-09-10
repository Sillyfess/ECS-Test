import { Entity } from './Entity';

export class EntityManager {
    private entities: Map<number, Entity> = new Map();
    private entityPool: Entity[] = [];
    private readonly poolSize: number = 100;

    constructor() {
        this.initializePool();
    }

    private initializePool(): void {
        for (let i = 0; i < this.poolSize; i++) {
            this.entityPool.push(new Entity());
        }
    }

    public createEntity(): Entity {
        let entity: Entity;
        
        if (this.entityPool.length > 0) {
            entity = this.entityPool.pop()!;
            entity.reset();
        } else {
            entity = new Entity();
        }

        this.entities.set(entity.id, entity);
        return entity;
    }

    public destroyEntity(entityId: number): void {
        const entity = this.entities.get(entityId);
        if (entity) {
            entity.destroy();
            this.entities.delete(entityId);
            
            if (this.entityPool.length < this.poolSize) {
                this.entityPool.push(entity);
            }
        }
    }

    public getEntity(entityId: number): Entity | undefined {
        return this.entities.get(entityId);
    }

    public getAllEntities(): Entity[] {
        return Array.from(this.entities.values());
    }

    public getActiveEntities(): Entity[] {
        return Array.from(this.entities.values()).filter(e => e.isAlive());
    }

    public getEntitiesWithComponentMask(mask: bigint): Entity[] {
        const result: Entity[] = [];
        for (const entity of this.entities.values()) {
            if (entity.isAlive() && (entity.getComponentMask() & mask) === mask) {
                result.push(entity);
            }
        }
        return result;
    }

    public clear(): void {
        for (const entity of this.entities.values()) {
            entity.destroy();
        }
        this.entities.clear();
    }

    public getEntityCount(): number {
        return this.entities.size;
    }
}